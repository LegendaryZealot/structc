#include "strext.h"
#include "elag.h"

// str_hash - Brian Kernighan�� Dennis Ritchie ����ݵ� hash�㷨
// str		: �ַ�������
// return	: ���ؼ�����hashֵ
unsigned 
str_hash(const char * str) {

	return 0u;
}

// str_icmp - �ַ��������ִ�Сд�ȽϺ���
// ls		: ��
// rs		: �Ҵ�
// return	: ls > rs ���� > 0; ... < 0; ... =0
int 
str_icmp(const char * ls, const char * rs) {
	return 0;
}

// str_dup - �ַ�������malloc����, ��Ҫ�Լ�free
// str		: �������Ĵ�
// return	: ���ؿ�����Ĵ�
char * 
str_dup(const char * str) {
	return NULL;
}

// str_freadend - �򵥵��ļ���ȡ��,���ȡ�������ļ����ݷ���, ��Ҫ�Լ�free
// path		: �ļ�·��
// return	: �����õ��ַ�������, ����NULL��ʾ��ȡʧ��
char * 
str_freadend(const char * path) {
	return NULL;
}

// str_fwrites - ��c��str����д�뵽path·�����ļ���
// path		: �ļ�·��
// str		: c�Ĵ�����
// return	: 0 is success, < 0 is error
int 
str_fwrites(const char * path, const char * str) {
	return 0;
}

// tstr_fappends - ��c��strд�뵽path·�����ļ���ĩβ
// path		: �ļ�·��
// str		: c�Ĵ�����
// return	: 0 is success, < 0 is error
int 
tstr_fappends(const char * pat, const char * str) {
	return 0;
}