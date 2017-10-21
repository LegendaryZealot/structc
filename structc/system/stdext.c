#include <stdio.h>
#include <jemalloc/jemalloc.h>

#define _NO_STDEXT_
#include <stdext.h>

//
// free_ext - free ��װ����
// ptr      : ͨ�����溯��������ڴ�, ����ͨ�������������
// return   : void
//
inline void 
free_ext(void * ptr) {
    je_free(ptr);
}

// �򵥴�����Ϣ��ӡ
static inline void _ext(void * ptr, size_t size) {
    fprintf(stderr, "alloc error ptr = %p, size = %zu!\n", ptr, size);
    fflush(stderr);
    abort();
}

//
// malloc_ext - malloc ��װ, ��װһЩҵ�����Դ���
// size     : ������ڴ��ֽ�
// return   : ���ؿ�ʹ�õ��ڴ��ַ.
//
inline void * 
malloc_ext(size_t size) {
    void * ptr = je_malloc(size);
    if (NULL == ptr)
        _ext(ptr, size);
    return ptr;
}

//
// calloc_ext - calloc ��װ, ��װһЩ����ҵ��
// num      : ����
// size     : ��С
// return   : ���ؿ����ڴ��ַ, ������0
//
inline void * 
calloc_ext(size_t num, size_t size) {
    void * ptr = je_calloc(num, size);
    if (NULL == ptr)
        _ext(ptr, num * size);
    return ptr;
}

//
// realloc_ext - realoc ��װ����, ��װһЩ����ҵ��
// ptr      : ���·�����ڴ��׵�ַ, NULL ��ͬ�� malloc
// size     : ���·�����ڴ��С
// return   : �������·���õ��µ�ַ����
//
inline void * 
realloc_ext(void * ptr, size_t size) {
    void * nptr = je_realloc(ptr, size);
    if (NULL == nptr) {
        je_free(ptr);   // ���пͻ�, �ڴ�ģ���Ѿ���δ������Ϊ�ı�Ե��
        _ext(ptr, size);   
    }
    return ptr;
}
