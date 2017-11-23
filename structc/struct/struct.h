#ifndef _H_STRUCT
#define _H_STRUCT

#include <stdext.h>

//
// cmp_f - �Ƚ���Ϊ������
//  : int add_cmp(const void * now, const void * node)
//  : (cmp_f)add_cmp
//
typedef int (* cmp_f)();

//
// new_f - ���ݹ��򹹽�����
//  : void * rb_new(void * node)
//  : (new_f)rb_new
//
typedef void * (* new_f)();

//
// die_f - ���ٵ�ǰ����ڵ�
//  : void list_die(void * node);
//  : (die_f)list_die  
//
typedef void (* die_f)();

#endif//_H_STRUCT