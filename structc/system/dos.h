#ifndef _H_DOS
#define _H_DOS

/*
    ƽ̨����ṩ�ļ򵥼��� 
 */

//
// ƽ̨��� : NOWOS == LINUS | WINDS | MACOS
// ��������Ŀǰֻ֧��, ���µ� WINDS CL �� LINUX GCC ����
//
#define LINUS   (1)
#define WINDS   (2)
#define MACOS   (3)

#if defined(_MSC_VER)
#   define NOWOS   WINDS
#elif defined(__APPLE__)
#   define NOWOS   MACOS
#elif defined(__GNUC__)
#   define NOWOS   LINUS
#else
#   error ERR (�����) OS
#endif

//
// ���Ի������ : DEBUG or RELEASE
// ISDEBUG EXISTS ��ʾ�ǲ��Ի���, ������ RELEASE ��������
//
#if defined(_DEBUG) || defined(DEBUG)
#   define ISDEBUG
#endif

//
// CPU ��� : x64 or x86
// ISX64() == true ��ʾ x64 ���� x86
//
inline _Bool ISX64() {
    return 8 == sizeof ((void *)0);
}

// 
// ��С�˼�� : ISBIG() == true ��ʾ���
//
inline _Bool ISBIG() {
    union { char c[sizeof(unsigned)]; unsigned u; } e = { { 's', '?', '?', 'b' } };
    return (char)e.u == 'b';
}

#endif//_H_DOS
