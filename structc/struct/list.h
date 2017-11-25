#ifndef _H_LIST
#define _H_LIST

#include <struct.h>

//
// list.h ����ͨ�õĵ������
//

struct $list {
    struct $list * next;
};

#define LIST_HEAD struct $list $node

struct list { 
    struct $list * root;    // �洢�����ͷ�ڵ�

    icmp_f fadd;     // �����в�������ִ�еķ���
    icmp_f fget;     // �����в�������ִ�еķ���
    node_f fdie;     // ������ɾ������ִ�еķ���
};

typedef struct list * list_t;

//
// list_create_ - ���� list ����
// fadd     : �������ݷ���
// fget     : ��ȡ���ݷ���
// fdie     : �������ݷ���
// return   : �����õ��������
//
inline list_t list_create_(icmp_f fadd, icmp_f fget, node_f fdie) {
    list_t list = malloc(sizeof(struct list));
    list->root = NULL;
    list->fadd = fadd;
    list->fget = fget;
    list->fdie = fdie;
    return list;
}

#define list_create(fadd, fget, fdie) \
list_create_((icmp_f)fadd, (icmp_f)fget, (node_f)fdie)

//
// list_delete - �����������ٲ���
// list     : ����������ṹ
// return   : void
//
extern void list_delete(list_t list);

#endif//_H_LIST