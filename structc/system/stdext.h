#ifndef _H_STDEXIT
#define _H_STDEXIT

/*
 stdxxx.h ��չ��
 ��Ҫ���� : ��չ��ǰ���ڴ������Ϊ
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//
// free_ - free ��װ����
// ptr      : ͨ�����溯��������ڴ�, ����ͨ�������������
// return   : void
//
extern void free_(void * ptr);

//
// malloc_ - malloc ��װ, ��װһЩҵ�����Դ���
// size     : ������ڴ��ֽ�
// return   : ���ؿ�ʹ�õ��ڴ��ַ.
//
extern void * malloc_(size_t size);

//
// calloc_ - calloc ��װ, ��װһЩ����ҵ��
// num      : ����
// size     : ��С
// return   : ���ؿ����ڴ��ַ, ������0
//
extern void * calloc_(size_t num, size_t size);

//
// realloc_ - realoc ��װ����, ��װһЩ����ҵ��
// ptr      : ���·�����ڴ��׵�ַ, NULL ��ͬ�� malloc
// size     : ���·�����ڴ��С
// return   : �������·���õ��µ�ַ����
//
extern void * realloc_(void * ptr, size_t size);

//
// ����ȫ������ free / malloc ����
//
#if !defined(_NO_STDEXT_)

#   undef  free
#   undef  malloc
#   undef  calloc
#   undef  realloc

#   define free    free_
#   define malloc  malloc_
#   define calloc  calloc_
#   define realloc realloc_

#endif

#endif//_H_STDEXIT
