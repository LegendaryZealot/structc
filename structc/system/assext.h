#ifndef _H_ASSEXT
#define _H_ASSEXT

//
// assert.h ����չ�ӿ�
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef _H_ELAG
#define _H_ELAG

//
// enum Elag int - ��������ֵȫ��״̬��
// >= 0 ��ʶSuccess״̬, < 0 ��ʶError״̬
//
enum Elag {
    EParse    = -8, //Э���������
    EClose    = -7, //�����ʧ��, ��ȡ���Ҳ�������
    EEmpty    = -6, //��������Ϊ��
    ETout     = -5, //������ʱ����
    EFd       = -4, //�ļ���ʧ��
    EAlloc    = -3, //�ڴ�������
    EParam    = -2, //�����������
    EBase     = -1, //�����������, ����λ�ô��󶼿�����

    SBase     = +0, //������ȷ����
};

#endif//_H_ELAG

//
// CERR - ��ӡ������Ϣ, �����һЩ���еĺ����
// CERR_EXIT - ��ӡ������Ϣ, ��ֱ��exit
// CERR_IF - �����ж��쳣�˳��ĸ�����
//
#define CERR(fmt, ...)                                                  \
fprintf(stderr, "[%s:%s:%d][%d:%s]" fmt "\r\n",                         \
    __FILE__, __func__, __LINE__, errno, strerror(errno), ##__VA_ARGS__)

#define CERR_EXIT(fmt, ...)     \
do {                            \
    CERR(fmt, ##__VA_ARGS__);   \
    exit(EXIT_FAILURE);         \
} while(0)


#define CERR_IF(cond)   \
if ((cond))             \
    CERR_EXIT(#cond)

//
// RETURN - ��ӡ������Ϣ, ��return����ָ�����
// val		: return�Ķ���, ����Ҫ return void; ʱ���� ',' �͹� or NIL
// fmt		: ˫���Ű����ĸ�ʽ���ַ���
// ...		: fmt�ж�Ӧ�Ĳ���
// return	: val
// 
#define NIL
#define RETURN(val, fmt, ...)   \
do {                            \
    CERR(fmt, ##__VA_ARGS__);   \
    return val;                 \
} while(0)

/*
 * ������һ�� �� DEBUG ģʽ�µĲ��Ժ�
 *
 * �÷� :
 * DEBUG_CODE({
 *	puts("debug start...");
 * });
 */
#ifndef DEBUG_CODE
# ifdef _DEBUG
#	define DEBUG_CODE(code) \
        do code while(0)
# else
#	define DEBUG_CODE(code) 
# endif	//	! _DEBUG
#endif	//	! DEBUG_CODE

#endif//_H_ASSEXT
