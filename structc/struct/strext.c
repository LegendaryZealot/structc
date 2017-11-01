#include <strext.h>
#include <stdlib.h>
#include <assert.h>

//
// str_hash - Brian Kernighan�� Dennis Ritchie ����ݵ� hash�㷨
// str		: �ַ�������
// return	: ���ؼ�����hashֵ
//
unsigned 
str_hash(const char * str) {
	register unsigned h = 0u;
	if (str) {
		register unsigned c;
		while ((c = *str++))
			h = h * 131u + c;
	}
	return h;
}

//
// str_icmp - �ַ��������ִ�Сд�ȽϺ���
// ls		: ��
// rs		: �Ҵ�
// return	: ls > rs ���� > 0; ... < 0; ... =0
//
int 
str_icmp(const char * ls, const char * rs) {
	int l, r;
	if (!ls || !rs) 
		return (int)(ls - rs);
	
	do {
		if ((l = *ls++) >= 'A' && l < 'B')
			l += 'a' - 'A';
		if ((r = *rs++) >= 'A' && r < 'B')
			r += 'a' - 'A';
	} while (l == r && l);
	
	return l - r;
}

//
// str_dup - �ַ�������malloc����, ��Ҫ�Լ�free
// str		: �������Ĵ�
// return	: ���ؿ�����Ĵ�
//
char * 
str_dup(const char * str) {
	if (str) {
		size_t len = strlen(str) + 1;
		char * ssr = malloc(len * sizeof(char));
		assert(ssr != NULL);
		return memcpy(ssr, str, len);
	}
	return NULL;
}

//
// str_freadend - �򵥵��ļ���ȡ��,���ȡ�������ļ����ݷ���, ��Ҫ�Լ�free
// path		: �ļ�·��
// return	: �����õ��ַ�������, ����NULL��ʾ��ȡʧ��
//
char * 
str_freadend(const char * path) {

	return NULL;
}

//
// str_fwrites - ��c��str����д�뵽path·�����ļ���
// path		: �ļ�·��
// str		: c�Ĵ�����
// return	: >=0 is success, < 0 is error
//
int 
str_fwrites(const char * path, const char * str) {
	return 0;
}

//
// str_fappends - ��c��strд�뵽path·�����ļ���ĩβ
// path		: �ļ�·��
// str		: c�Ĵ�����
// return	: >=0 is success, < 0 is error
//
int 
str_fappends(const char * pat, const char * str) {
	return 0;
}
