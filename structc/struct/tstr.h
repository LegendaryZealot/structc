#ifndef _H_TSTR
#define _H_TSTR

#include <strext.h>

#ifndef _STRUCT_TSTR

struct tstr {
    unsigned len;   // ����
    unsigned cap;   // ����
    char * str;     // �ַ���
};

typedef struct tstr * tstr_t;

//
// TSTR_CREATE - ���ϴ���tstr_t�ṹ
// TSTR_DELETE - �ͷŶ���tstr_t�ṹ
// var  : ������
//
#define TSTR_CREATE(var) \
struct tstr var[1] = { { 0, 0, NULL } }
#define TSTR_DELETE(var) \
free((var)->str)

#define _STRUCT_TSTR
#endif//_STRUCT_TSTR

//
// tstr_t ��������, �����c��tstr������һ��tstr_t�ṹ���ַ���
// str		: ���������ַ���
// len		: �������ĳ���
// return	: ���ش����õ��ַ���,�ڴ治����ӡ��־�˳�����
//
extern tstr_t tstr_create(const char * str, unsigned len);
extern tstr_t tstr_creates(const char * str);

//
// tstr_delete - tstr_t �ͷź���
// tstr		: ���ͷŵĴ��ṹ
// return   : void
//
extern void tstr_delete(tstr_t tstr);

//
// ��tstr_t���ṹ������ַ���, �ڴ����ʧ���ڲ����Լ�����
// c		: ������ӵ�char
// str		: ��ӵ�c��
// sz		: ��Ӵ��ĳ���
//
extern void tstr_appendc(tstr_t tstr, int c);
extern void tstr_appends(tstr_t tstr, const char * str);
extern void tstr_appendn(tstr_t tstr, const char * str, unsigned sz);

//
// tstr_cstr - ͨ��cstr_t���õ�һ��c�Ĵ���'\0'��β
// tstr		: tstr_t ��
// return	: ���ع����õ�c�Ĵ�, �ڴ��ַtstr->str
//
extern char * tstr_cstr(tstr_t tstr);

//
// tstr_expand - Ϊ��ǰ�ַ�������, ���ڵͼ�api
// tstr		: �ɱ��ַ���
// len		: ���ݵĳ���
// return	: tstr->str + tstr->len λ�õĴ�
//
char * tstr_expand(tstr_t tstr, unsigned len);

//
// tstr_popup - ���ַ���ͷ����len�����ַ�
// tstr		: �ɱ��ַ���
// len		: �����ĳ���
// return	: void
//
extern void tstr_popup(tstr_t tstr, unsigned len);

//
// tstr_dupstr - �õ�һ�������c�Ĵ�, ��Ҫ�Լ��º�free
// tstr		: tstr_t ��
// return	: ���ش����õ�c��
//
extern char * tstr_dupstr(tstr_t tstr);

//
// tstr_printf - ���� sprintf ��䷽ʽд������
// tstr     : tstr_t ��
// format   : ����ʽ���Ĵ�
// ...      : �ȴ�����ı���
// return   : ���ش����õ�C�ַ�������
//
extern char * tstr_printf(tstr_t tstr, const char * format, ...);

#endif//_H_TSTR
