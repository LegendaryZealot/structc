#ifndef _H_STREXT
#define _H_STREXT

/*
	�̳� : string.h
	���� : ��չ string.h �в��ֹ���, ����ҵ������
 */

#include <string.h>

//
// str_hash - Brian Kernighan�� Dennis Ritchie ����ݵ� hash�㷨
// str		: �ַ�������
// return	: ���ؼ�����hashֵ
//
extern unsigned str_hash(const char * str);

//
// str_icmp - �ַ��������ִ�Сд�ȽϺ���
// ls		: ��
// rs		: �Ҵ�
// return	: ls > rs ���� > 0; ... < 0; ... =0
//
extern int str_icmp(const char * ls, const char * rs);

//
// str_dup - �ַ�������malloc����, ��Ҫ�Լ�free
// str		: �������Ĵ�
// return	: ���ؿ�����Ĵ�
//
extern char * str_dup(const char * str);

//
// str_freadend - �򵥵��ļ���ȡ��,���ȡ�������ļ����ݷ���, ��Ҫ�Լ�free
// path		: �ļ�·��
// return	: �����õ��ַ�������, ����NULL��ʾ��ȡʧ��
//
extern char * str_freadend(const char * path);

//
// str_fwrites - ��c��str����д�뵽path·�����ļ���
// path		: �ļ�·��
// str		: c�Ĵ�����
// return	: >=0 is success, < 0 is error
//
extern int str_fwrites(const char * path, const char * str);

//
// tstr_fappends - ��c��strд�뵽path·�����ļ���ĩβ
// path		: �ļ�·��
// str		: c�Ĵ�����
// return	: >=0 is success, < 0 is error
//
extern int tstr_fappends(const char * pat, const char * str);

#endif//_H_STREXT
