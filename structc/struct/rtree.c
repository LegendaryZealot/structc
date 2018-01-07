#include <rtree.h>

//
// struct $rtree �ṹ����������
// r    : ��ǰ���
// p    : �����
// c    : ��ǰ�����ɫ, 1 ��ʾ black, 0 ��ʾ red
//
#define rtree_parent(r)      ((struct $rtree *)((r)->parentc & ~3))
#define rtree_color(r)       (1 & (r)->parent_color)
#define rtree_is_red(r)      (!rtree_color(r))
#define rtree_is_black(r)    rtree_color(r)
#define rtree_set_red(r)     (r)->parentc &= ~1
#define rtree_set_black(r)   (r)->parentc |= 1

inline void rtree_set_parent(struct $rtree * r, struct $rtree * p) {
    r->parentc = (r->parentc & 3) | (uintptr_t)p;
}

inline void rb_set_color(struct $rtree * r, int color) {
    r->parentc = (r->parentc & ~1) | (1 & color);
}

static inline int _rtree_default_cmp(const void * ln, const void * rn) {
    return (int)((intptr_t)ln - (intptr_t)rn);
}

//
// rtee_create - ����һ��������ṹ
// fcmp     : �����Һ���
// fnew     : ��㹹�캯��
// fdie     : ������ٺ���
// return   : ���ع��������
//
inline rtree_t 
rtree_create_(icmp_f fcmp, vnew_f fnew, node_f fdie) {
    rtree_t tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->fcmp = fcmp ? fcmp : _rtree_default_cmp;
    tree->fnew = fnew;
    tree->fdie = fdie;
    return tree;
}

// ��ϸɾ������, ���ڱ�ջ����
static void _rtree_delete(struct $rtree * root, node_f fdie) {
    if (root->left)
        _rtree_delete(root->left, fdie);
    if (root->right)
        _rtree_delete(root->right, fdie);
    fdie(root);
}

//
// rtree_delete - ��������ٺ���
// tree     : �����ٵĺ����
// return   : void
//
inline void 
rtree_delete(rtree_t tree) {
    if (NULL == tree) return;
    if (tree->root && tree->fdie)
        _rtree_delete(tree->root, tree->fdie);

    tree->root = NULL;
    free(tree);
}

// ����ʱ����һ���½��
inline struct $rtree * rtree_new(rtree_t tree, void * pack) {
    struct $rtree * node = tree->fnew ? tree->fnew(pack) : pack;
    memset(node, 0, sizeof *node);
    return node;
}

