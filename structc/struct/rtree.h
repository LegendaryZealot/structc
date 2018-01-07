#ifndef _H_RTREE
#define _H_RTREE

#include <struct.h>

//
// �����ͨ�ýṹ, ��Ҫ�� $RTREE ���ڽṹ��ͷ��λ
//

struct $rtree {
    uintptr_t parentc;
    struct $rtree * left;
    struct $rtree * right;
};

#define $RTREE struct $rtree $node;

typedef struct {
    struct $rtree * root;

    icmp_f fcmp;
    vnew_f fnew;
    node_f fdie;

    icmp_f fget;
} * rtree_t;

//
// rtee_create - ����һ��������ṹ
// fcmp     : �����Һ���
// fnew     : ��㹹�캯��
// fdie     : ������ٺ���
// return   : ���ع��������
//
#define rtree_create(fcmp, fnew, fdie) \
rtree_create_((icmp_f)(fcmp), (vnew_f)(fnew), (node_f)(fdie))

extern rtree_t rtree_create_(icmp_f fcmp, vnew_f fnew, node_f fdie);

//
// rtree_delete - ��������ٺ���
// tree     : �����ٵĺ����
// return   : void
//
extern void rtree_delete(rtree_t tree);

//
// rtree_search - ��������Һ���
// tree     : �����ҵĺ�����ṹ
// return   : ���ز��ҵĽڵ�
//
extern void * rtree_search(rtree_t tree, void * pack);

//
// rtree_insert - ������в���ڵ� fnew(pack)
// tree     : ������ṹ
// pack     : ����������ṹ
// return   : void
//
extern void rtree_insert(rtree_t tree, void * pack);

//
// rtree_remove - �������ɾ���ڵ�
// tree     : ������ṹ
// pack     : ��ɾ�������ṹ
// return   : void
//
extern void rtree_remove(rtree_t tree, void * pack);

#endif//_H_RTREE