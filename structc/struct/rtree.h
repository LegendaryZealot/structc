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
} * rtree_t;

//
// rtee_create - ����һ��������ṹ
// fcmp     : �����Һ���
// fnew     : ��㹹�캯��
// fdie     : ������ٺ���
// return   : ���ع��������
//
#define rtee_create(fcmp, fnew, fdie) \
rtree_create_((icmp_f)(fcmp), (vnew_f)(fnew), (node_f)(fdie))

extern rtree_t rtree_create_(icmp_f fcmp, vnew_f fnew, node_f fdie);

//
// rtree_delete - ��������ٺ���
// tree     : �����ٵĺ����
// return   : void
//
extern void rtree_delete(rtree_t tree);

#endif//_H_RTREE