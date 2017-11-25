#ifndef _H_STRUCT
#define _H_STRUCT

#include <stdext.h>

//
// icmp_f - �Ƚ���Ϊ������
//  : int add_cmp(const void * now, const void * node)
//
typedef int (* icmp_f)();

//
// vnew_f - ���ݹ��򹹽�����
//  : void * rb_new(void * node)
//
typedef void * (* vnew_f)();

//
// node_f - ���ٵ�ǰ����ڵ�
//  : void list_die(void * node); 
//
typedef void (* node_f)(void * node);

//
// each_f - each ѭ������, arg �ⲿ����, node ���ڲ����
//  : int dict_echo(struct dict * node, void * arg) { return 0; }
//
typedef int (* each_f)(void * node, void * arg);

//
// start_f - pthread create func
//  : int * run(int * arg)
//
typedef void *  (* start_f)(void * arg);

#endif//_H_STRUCT