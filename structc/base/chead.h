#ifndef _H_CHEAD
#define _H_CHEAD

#include <time.h>
#include <stdio.h>

//
// STR - ���˫���ŵĺ� 
// v    : �����˫���ŵ���
//
#define STR_(v) #v
#define STR(v)	STR_(v)

//
// EXTERN_RUN - �򵥵�����, ������ʹ�õĺ�
// ftest    : ��Ҫִ�еĺ�������
// ...      : �ɱ����, ����
//
#define EXTERN_RUN(ftest, ...)  \
do {							\
    extern void ftest();		\
    ftest (__VA_ARGS__);		\
} while(0)

//
// TEST_CODE - ���Դ����, �������ʱ����Ϣ
// code : { ... } �����Ĵ����
//
#define TEST_CODE(code)                                                 \
do {                                                                    \
    clock_t $s = clock();                                               \
    code                                                                \
    double $e = (double)clock();                                        \
    printf("Code test run time:%lfs.\n", ($e - $s) / CLOCKS_PER_SEC);   \
} while (0)

//
// TEST_FUNC - ���ڵ�Ԫ���Ժ���, ִ�в��������ʱ��
// ftest    : ��Ҫִ�еĲ��Ժ�������
// ...      : �ɱ����, ����
//
#define TEST_FUNC(ftest, ...)                                           \
do {                                                                    \
    clock_t $s = clock();                                               \
    extern void ftest();                                                \
    ftest (##__VA_ARGS__);                                              \
    double $e = (double)clock();                                        \
    printf(STR(ftest) " run time:%lfs.\n", ($e - $s) / CLOCKS_PER_SEC); \
} while(0)

#endif//_H_CHEAD
