#define _NO_STDEXT_
#include "stdext.h"

#include <jemalloc/jemalloc.h>

//
// free_ - free ��װ����
// ptr      : ͨ�����溯��������ڴ�, ����ͨ�������������
// return   : void
//
inline void free_(void * ptr) {
    je_free(ptr);
}

// ���ڴ治���⴦��
inline void * _mcheck_(void * ptr, size_t size) {
    if (NULL == ptr) {
        fprintf(stderr, 
            "Out of memory trying to allocate <%p, %zu>\n",
            ptr, size);
        fflush(stderr);
        abort();
    }
    return ptr;
}

//
// malloc_ - malloc ��װ, ��װһЩҵ�����Դ���
// size     : ������ڴ��ֽ�
// return   : ���ؿ�ʹ�õ��ڴ��ַ.
//
inline void * malloc_(size_t size) {
    void * ptr = je_malloc(size);
    return _mcheck_(ptr, size);
}

//
// calloc_ - calloc ��װ, ��װһЩ����ҵ��
// num      : ����
// size     : ��С
// return   : ���ؿ����ڴ��ַ, ������0
//
inline void * calloc_(size_t num, size_t size) {
    void * ptr = je_calloc(num, size);
    return _mcheck_(ptr, size);
}

//
// realloc_ - realoc ��װ����, ��װһЩ����ҵ��
// ptr      : ���·�����ڴ��׵�ַ, NULL ��ͬ�� malloc
// size     : ���·�����ڴ��С
// return   : �������·���õ��µ�ַ����
//
inline void * realloc_(void * ptr, size_t size) {
    void * ntr = je_realloc(ptr, size);
    return _mcheck_(ntr, size);
}
