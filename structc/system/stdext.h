#ifndef _H_STDEXIT
#define _H_STDEXIT

/*
 stdxxx.h ��չ��

 ��Ҫ���� : ��չ�ڴ����

 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//
// free_ext - free ��װ����
// ptr      : ͨ�����溯��������ڴ�, ����ͨ�������������
// return   : void
//
extern void free_ext(void * ptr);

//
// malloc_ext - malloc ��װ, ��װһЩҵ�����Դ���
// size     : ������ڴ��ֽ�
// return   : ���ؿ�ʹ�õ��ڴ��ַ.
//
extern void * malloc_ext(size_t size);

//
// calloc_ext - calloc ��װ, ��װһЩ����ҵ��
// num      : ����
// size     : ��С
// return   : ���ؿ����ڴ��ַ, ������0
//
extern void * calloc_ext(size_t num, size_t size);

//
// realloc_ext - realoc ��װ����, ��װһЩ����ҵ��
// ptr      : ���·�����ڴ��׵�ַ, NULL ��ͬ�� malloc
// size     : ���·�����ڴ��С
// return   : �������·���õ��µ�ַ����
//
extern void * realloc_ext(void * ptr, size_t size);

//
// ����ȫ������ free / malloc ����
//
#if !defined(_NO_STDEXT_)

#   undef  free
#   undef  malloc
#   undef  calloc
#   undef  realloc

#   define free    free_ext
#   define malloc  malloc_ext
#   define calloc  calloc_ext
#   define realloc realloc_ext

#endif

#endif//_H_STDEXIT
