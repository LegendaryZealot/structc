#ifndef _H_DOS
#define _H_DOS

/*
    ƽ̨����ṩ�ļ򵥼��� 
 */

//
// ����ƽ̨��� : winds cl -> WCL | LGCC -> linux gcc
// Ŀǰ����ֻ֧��, ���µ� WINDS CL �� LINUX GCC ����
//
#if defined(__GNUC__)
#   define LGCC (1)
#elif defined(_MSC_VER)
#   define WCL  (2)
#else
#   error BUILD (�����) S
#endif

//
// ���Ի������ : DEBUG or RELEASE
// ISDEBUG EXISTS ��ʾ�ǲ��Ի���, ������ RELEASE ��������
//
#if defined(_DEBUG) || defined(DEBUG)
#   define ISDEBUG
#endif

#if defined(WCL)

//
// CPU ��� : x64 or x86
// ISX64 defined ��ʾ x64 ���� x86
//
#   if defined(_M_ARM64) || defined(_M_X64)
#       define ISX64
#   endif

#   if defined(_M_PPC)
#       define ISBENIAN
#endif

#endif

#if defined(LGCC)

#   if defined(__x86_64__)
#       define ISX64
#   endif

// 
// ��С�˼�� : ISBENIAN defined ��ʾ���
//
#   if defined(__BIG_ENDIAN__) || defined(__BIG_ENDIAN_BITFIELD)
#       define ISBENIAN
#   endif

#endif

#endif//_H_DOS
