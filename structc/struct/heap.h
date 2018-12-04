#ifndef _H_HEAP
#define _H_HEAP

#include "struct.h"

//
// head_t �ѵ����ͽṹ
//
typedef struct heap * heap_t;

//
// heap_create - �������Ϲ���Ķ�
// fcmp     : �Ƚ���Ϊ, ���� fcmp() <= 0
// return   : ���ش����õĶѶ���
//
extern heap_t heap_create(cmp_f fcmp);

//
// heap_delete - ���ٶ�
// h        : �Ѷ���
// fdie     : ������Ϊ, Ĭ�� NULL
// return   : void
//
extern void heap_delete(heap_t h, node_f fdie);

//
// heap_insert - �Ѳ�������
// h        : �Ѷ���
// node     : ��������
// return   : void
//
extern void heap_insert(heap_t h, void * node);

//
// heap_remove - ��ɾ������
// h        : �Ѷ���
// node     : ��������
// return   : �ҵ��Ķѽڵ�
//
extern void * heap_remove(heap_t h, void * node);

//
// heap_top - �鿴�Ѷ��������
// h        : �Ѷ���
// return   : �Ѷ��ڵ�
//
extern void * heap_top(heap_t h);

//
// heap_top - ժ���Ѷ��������
// h        : �Ѷ���
// return   : ���ضѶ��ڵ�
//
extern void * heap_pop(heap_t h);

#endif//_H_HEAP
