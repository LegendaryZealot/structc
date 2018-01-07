#include <rtree.h>

//
// struct $rtree �ṹ����������
// r    : ��ǰ���
// p    : �����
// c    : ��ǰ�����ɫ, 1 ��ʾ black, 0 ��ʾ red
//
#define rtree_parent(r)      ((struct $rtree *)((r)->parentc & ~3))
#define rtree_color(r)       ((r)->parentc & 1)
#define rtree_is_red(r)      (!rtree_color(r))
#define rtree_is_black(r)    rtree_color(r)
#define rtree_set_red(r)     (r)->parentc &= ~1
#define rtree_set_black(r)   (r)->parentc |= 1

inline void rtree_set_parent(struct $rtree * r, struct $rtree * p) {
    r->parentc = (r->parentc & 3) | (uintptr_t)p;
}

inline void rtree_set_color(struct $rtree * r, int color) {
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

//
// rtree_search - ��������Һ���
// tree     : �����ҵĺ�����ṹ
// return   : ���ز��ҵĽڵ�
//
void * 
rtree_search(rtree_t tree, void * pack) {
    icmp_f fcmp;
    struct $rtree * node;
    if (!tree) return NULL;

    fcmp = tree->fget ? tree->fget : tree->fcmp;
    node = tree->root;
    while (node) {
        int diff = fcmp(node, pack);
        if (diff == 0)
            break;
        //
        // tree left less, right greater 
        //
        node = diff > 0 ? node->left : node->right;
    }
    return node;
}

/*
 ���������

    ����1: �ڵ��Ǻ�ɫ���ɫ
    ����2: ���Ǻ�ɫ
    ����3: ���� NULL Ҷ�Ӷ��Ǻ�ɫ
    ����4: ����һ�ڵ㵽��ÿ��Ҷ�ӵ����м�·����������ͬ��Ŀ�ĺ�ɫ�ڵ�
    ����5: ��ÿ��Ҷ�ӵ���������·���ϲ��������������ĺ�ɫ�ڵ�
 
 ��ϸ�Ŀ��Բ鿴, ���沩���ĺ����ϵ������
    https://www.cnblogs.com/skywang12345/p/3624177.html

*/

/* 
 * �Ժ�����Ľڵ� [x] ��������ת
 *
 * ����ʾ��ͼ (�Խڵ� x ��������):
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \    --- (����) -->          / \
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly  
 *
 */
static void _rtree_left_rotate(rtree_t tree, struct $rtree * x) {
    // ���� [x] ���Һ���Ϊ [y]
    struct $rtree * y = x->right;
    struct $rtree * xparent = rtree_parent(x);

    // �� [y������] ��Ϊ [x���Һ���]��
    x->right = y->left;
    // ��� y������ �ǿգ��� [x] ��Ϊ [y�����ӵĸ���]
    if (y->left != NULL)
        rtree_set_parent(y->left, x);

    // �� [x�ĸ���] ��Ϊ [y�ĸ���]
    rtree_set_parent(y, xparent);

    if (xparent == NULL)
        tree->root = y;             // ��� [x�ĸ���] �ǿսڵ�, �� [y] ��Ϊ���ڵ�
    else {
        if (xparent->left == x)
            xparent->left = y;      // ��� [x] �������ڵ������, �� [y] ��Ϊ [x�ĸ��ڵ������]
        else
            xparent->right = y;     // ��� [x] �������ڵ������, �� [y] ��Ϊ [x�ĸ��ڵ������]
    }

    // �� [x] ��Ϊ [y������]
    y->left = x;
    // �� [x�ĸ��ڵ�] ��Ϊ [y]
    rtree_set_parent(x, y);
}

/* 
 * �Ժ�����Ľڵ� [y] ��������ת
 *
 * ����ʾ��ͼ(�Խڵ�y��������)��
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \     --- (����) -->          /  \
 *        x   ry                           lx   y  
 *       / \                                   / \
 *      lx  rx                                rx  ry
 * 
 */
static void _rtree_right_rotate(rtree_t tree, struct $rtree * y) {
    // ���� [x] �ǵ�ǰ�ڵ�����ӡ�
    struct $rtree * x = y->left;
    struct $rtree * yparent = rtree_parent(y);

    // �� [x���Һ���] ��Ϊ [y������]
    y->left = x->right;
    // ��� x���Һ��� ��Ϊ�յĻ����� [y] ��Ϊ [x���Һ��ӵĸ���]
    if (x->right != NULL)
        rtree_set_parent(x->right, y);

    // �� [y�ĸ���] ��Ϊ [x�ĸ���]
    rtree_set_parent(x, yparent);
    if (yparent == NULL) 
        tree->root = x;				// ��� [y�ĸ���] �ǿսڵ�, �� [x] ��Ϊ���ڵ�
    else {
        if (y == yparent->right)
            yparent->right = x;		// ��� [y] �������ڵ���Һ���, �� [x] ��Ϊ [y�ĸ��ڵ���Һ���]
        else
            yparent->left = x;		// ��� [y] �������ڵ������, �� [x] ��Ϊ [x�ĸ��ڵ������]
    }

    // �� [y] ��Ϊ [x���Һ���]
    x->right = y;
    // �� [y�ĸ��ڵ�] ��Ϊ [x]
    rtree_set_parent(y, x);
}

/*
 * �����������������
 *
 * ���������в���ڵ�֮��(ʧȥƽ��), �ٵ��øú���.
 * Ŀ���ǽ������������һ�ź����.
 *
 * ����˵��:
 *     tree ������ĸ�
 *     node ����Ľ��        // ��Ӧ <<�㷨����>> �е� z
 */
static void _rtree_insert_fixup(rtree_t tree, struct $rtree * node) {
    struct $rtree * parent, * gparent, * uncle;

    // �� [���ڵ�] ���ڣ����� [���ڵ�] ����ɫ�Ǻ�ɫ
    while ((parent = rtree_parent(node)) && rtree_is_red(parent)) {
        gparent = rtree_parent(parent);

        //�� [���ڵ�] �� [�游�ڵ������]
        if (parent == gparent->left) {
            // Case 1 ����: ����ڵ��Ǻ�ɫ
            uncle = gparent->right;
            if (uncle && rtree_is_red(uncle)) {
                rtree_set_black(uncle);
                rtree_set_black(parent);
                rtree_set_red(gparent);
                node = gparent;
                continue;
            }

            // Case 2 ����: �����Ǻ�ɫ, �ҵ�ǰ�ڵ����Һ���
            if (parent->right == node) {
                _rtree_left_rotate(tree, parent);
                uncle = parent;
                parent = node;
                node = uncle;
            }

            // Case 3 ����: �����Ǻ�ɫ, �ҵ�ǰ�ڵ�������
            rtree_set_black(parent);
            rtree_set_red(gparent);
            _rtree_right_rotate(tree, gparent);
        } else { // �� [z�ĸ��ڵ�] �� [z���游�ڵ���Һ���]
            // Case 1 ����: ����ڵ��Ǻ�ɫ
            uncle = gparent->left;
            if (uncle && rtree_is_red(uncle)) {
                rtree_set_black(uncle);
                rtree_set_black(parent);
                rtree_set_red(gparent);
                node = gparent;
                continue;
            }

            // Case 2 ����: �����Ǻ�ɫ, �ҵ�ǰ�ڵ�������
            if (parent->left == node) {
                _rtree_right_rotate(tree, parent);
                uncle = parent;
                parent = node;
                node = uncle;
            }

            // Case 3 ����: �����Ǻ�ɫ, �ҵ�ǰ�ڵ����Һ���
            rtree_set_black(parent);
            rtree_set_red(gparent);
            _rtree_left_rotate(tree, gparent);
        }
    }

    // �����ڵ���Ϊ��ɫ
    rtree_set_black(tree->root);
}

// ����ʱ����һ���½ڵ�
static inline struct $rtree * _rtree_new(rtree_t tree, void * pack) {
    struct $rtree * node = tree->fnew ? tree->fnew(pack) : pack;
    // Ĭ�Ϲ����ڵ��Ǻ�ɫ��
    memset(node, 0, sizeof *node);
    return node;
}

//
// rtree_insert - ������в����� fnew(pack)
// tree     : ������ṹ
// pack     : ����������ṹ
// return   : void
//
void 
rtree_insert(rtree_t tree, void * pack) {
    icmp_f fcmp;
    struct $rtree * node, * x, * y;
    if (!tree || !pack) return;
    
    y = NULL;
    x = tree->root;
    fcmp = tree->fcmp;
    // 1. �������ڵ�, �����ýڵ����ɫΪ��ɫ
    node = _rtree_new(tree, pack);

    // 2. �����������һ�Ŷ��������, ���ڵ���ӵ������������. Ĭ�� ��С����
    while (x) {
        y = x;
        if (fcmp(x, node) > 0)
            x = x->left;
        else
            x = x->right;
    }
    rtree_set_parent(node, y);

    if (NULL == y)
        tree->root = node;              // ��� 1: �� y�ǿսڵ�, �� node��Ϊ��
    else {
        if (fcmp(y, node) > 0)
            y->left = node;             // ��� 2: �� "node��������ֵ" < "y��������ֵ", �� [node] ��Ϊ [y������]
        else
            y->right = node;            // ��� 3���� "node��������ֵ" >= "y��������ֵ", �� [node] ��Ϊ [y���Һ���] 
    }

    // 3. ������������Ϊһ�Ŷ��������
    _rtree_insert_fixup(tree, node);
}

/*
 * �����ɾ����������
 *
 * �ڴӺ������ɾ������ڵ�֮��(�����ʧȥƽ��), �ٵ��øú���.
 * Ŀ���ǽ������������һ�ź����.
 *
 * ����˵��:
 *     tree ������ĸ�
 *     node �������Ľڵ�
 */
static void _rtree_remove_fixup(rtree_t tree, struct $rtree * node, struct $rtree * parent) {
    struct $rtree * other;

    while ((!node || rtree_is_black(node)) && node != tree->root) {
        if (parent->left == node) {
            other = parent->right;
            if (rtree_is_red(other)) {
                // Case 1: x���ֵ� w �Ǻ�ɫ��  
                rtree_set_black(other);
                rtree_set_red(parent);
                _rtree_left_rotate(tree, parent);
                other = parent->right;
            }
            if ((!other->left || rtree_is_black(other->left)) &&
                (!other->right || rtree_is_black(other->right))) {
                // Case 2: x���ֵ� w �Ǻ�ɫ, �� w����������Ҳ���Ǻ�ɫ��  
                rtree_set_red(other);
                node = parent;
                parent = rtree_parent(node);
            } else {
                if (!other->right || rtree_is_black(other->right)) {
                    // Case 3: x���ֵ� w �Ǻ�ɫ��, ���� w�������Ǻ�ɫ, �Һ���Ϊ��ɫ  
                    rtree_set_black(other->left);
                    rtree_set_red(other);
                    _rtree_right_rotate(tree, other);
                    other = parent->right;
                }
                // Case 4: x���ֵ� w �Ǻ�ɫ��, ���� w���Һ����Ǻ�ɫ��, ����������ɫ
                rtree_set_color(other, rtree_color(parent));
                rtree_set_black(parent);
                rtree_set_black(other->right);
                _rtree_left_rotate(tree, parent);
                node = tree->root;
                break;
            }
        } else {
            other = parent->left;
            if (rtree_is_red(other)) {
                // Case 1: x ���ֵ� w �Ǻ�ɫ��  
                rtree_set_black(other);
                rtree_set_red(parent);
                _rtree_right_rotate(tree, parent);
                other = parent->left;
            }
            if ((!other->left || rtree_is_black(other->left)) &&
                (!other->right || rtree_is_black(other->right))) {
                // Case 2: x ���ֵ� w �Ǻ�ɫ, �� w���������� Ҳ���Ǻ�ɫ��  
                rtree_set_red(other);
                node = parent;
                parent = rtree_parent(node);
            } else {
                if (!other->left || rtree_is_black(other->left)) {
                    // Case 3: x���ֵ� w �Ǻ�ɫ��, ���� w�������Ǻ�ɫ, �Һ���Ϊ��ɫ
                    rtree_set_black(other->right);
                    rtree_set_red(other);
                    _rtree_left_rotate(tree, other);
                    other = parent->left;
                }
                // Case 4: x���ֵ� w �Ǻ�ɫ��, ���� w���Һ����Ǻ�ɫ��, ����������ɫ.
                rtree_set_color(other, rtree_color(parent));
                rtree_set_black(parent);
                rtree_set_black(other->left);
                _rtree_right_rotate(tree, parent);
                node = tree->root;
                break;
            }
        }
    }
    if (node)
        rtree_set_black(node);
}

//
// rtree_remove - �������ɾ���ڵ�
// tree     : ������ṹ
// pack     : ��ɾ�������ṹ
// return   : void
//
void 
rtree_remove(rtree_t tree, void * pack) {
    int color;
    struct $rtree * child, * parent, * node = pack;
    if (NULL != tree) return;

    // ��ɾ���ڵ�� "���Һ��Ӷ���Ϊ��" �����
    if (NULL != node->left && node->right != NULL) {
        // ��ɾ�ڵ�ĺ�̽ڵ�. (��Ϊ "ȡ���ڵ�")
        // ������ȡ�� "��ɾ�ڵ�" ��λ��, Ȼ���ٽ� "��ɾ�ڵ�" ȥ��
        struct $rtree * replace = node;

        // ��ȡ��̽ڵ�
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node�ڵ�" ���Ǹ��ڵ�(ֻ�и��ڵ㲻���ڸ��ڵ�)
        if ((parent = rtree_parent(node))) {
            if (parent->left == node)
                parent->left = replace;
            else
                parent->right = replace;
        } else // "node�ڵ�" �Ǹ��ڵ�, ���¸��ڵ�
            tree->root = replace;

        // child �� "ȡ���ڵ�" ���Һ���, Ҳ����Ҫ "�����Ľڵ�"
        // "ȡ���ڵ�" �϶�����������! ��Ϊ����һ����̽ڵ�
        child = replace->right;
        parent = rtree_parent(replace);
        // ���� "ȡ���ڵ�" ����ɫ
        color = rtree_color(replace);

        // "��ɾ���ڵ�" �� "���ĺ�̽ڵ�ĸ��ڵ�"
        if (parent == node)
            parent = replace; 
        else {
            // child��Ϊ��
            if (child)
                rtree_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rtree_set_parent(node->right, replace);
        }

        rtree_set_parent(replace, rtree_parent(node));
        rtree_set_color(replace, rtree_color(node));
        replace->left = node->left;
        rtree_set_parent(node->left, replace);

        goto _ret;
    }

    if (NULL != node->left)
        child = node->left;
    else 
        child = node->right;

    parent = rtree_parent(node);
    // ���� "ȡ���ڵ�" ����ɫ
    color = rtree_color(node);

    if (child)
        rtree_set_parent(child, parent);

    // "node�ڵ�" ���Ǹ��ڵ�
    if (NULL == parent)
        tree->root = child;
    else {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }

_ret:
    if (color) // ��ɫ������µ�����ϵ, �����ٽڵ����
        _rtree_remove_fixup(tree, child, parent);
    if (tree->fdie)
        tree->fdie(node);
}
