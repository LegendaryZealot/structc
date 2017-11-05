#include <stdio.h>
#include <jemalloc/jemalloc.h>

//
// ���뵼��
// gcc -I./structc/system �������Ӳ��� -ljemalloc
//
#define _NO_STDEXT_
#include "stdext.h"

//
// free_ - free ��װ����
// ptr      : ͨ�����溯��������ڴ�, ����ͨ�������������
// return   : void
//
inline void 
free_(void * ptr) {
    je_free(ptr);
}

// �򵥴�����Ϣ��ӡ
static inline void _ext(void * ptr, size_t size) {
    fprintf(stderr, "alloc error ptr = %p, size = %zu!\n", ptr, size);
    fflush(stderr);
    abort();
}

//
// malloc_ - malloc ��װ, ��װһЩҵ�����Դ���
// size     : ������ڴ��ֽ�
// return   : ���ؿ�ʹ�õ��ڴ��ַ.
//
inline void * 
malloc_(size_t size) {
    void * ptr = je_malloc(size);
    if (NULL == ptr)
        _ext(ptr, size);
    return ptr;
}

//
// calloc_ - calloc ��װ, ��װһЩ����ҵ��
// num      : ����
// size     : ��С
// return   : ���ؿ����ڴ��ַ, ������0
//
inline void * 
calloc_(size_t num, size_t size) {
    void * ptr = je_calloc(num, size);
    if (NULL == ptr)
        _ext(ptr, num * size);
    return ptr;
}

//
// realloc_ - realoc ��װ����, ��װһЩ����ҵ��
// ptr      : ���·�����ڴ��׵�ַ, NULL ��ͬ�� malloc
// size     : ���·�����ڴ��С
// return   : �������·���õ��µ�ַ����
//
inline void * 
realloc_(void * ptr, size_t size) {
    void * nptr = je_realloc(ptr, size);
    if (NULL == nptr) {
        je_free(ptr);   // ���п���, �ڴ�ģ���Ѿ���δ������Ϊ�ı�Ե��
        _ext(ptr, size);   
    }
    return ptr;
}
