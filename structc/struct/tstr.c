#include <tstr.h>

// �ַ��������ĳ�ʼ����С
#define _UINT_TSTR  (32u)

//
// tstr_expand - Ϊ��ǰ�ַ�������, ���ڵͼ�api
// tstr		: �ɱ��ַ���
// len		: ���ݵĳ���
// return	: tstr->str + tstr->len λ�õĴ�
//
char * 
tstr_expand(tstr_t tstr, size_t len) {
    size_t cap = tstr->cap;
    if ((len += tstr->len) > cap) {
        for (cap = cap < _UINT_TSTR ? _UINT_TSTR : cap; cap < len; cap <<= 1)
            ;
        // ��Ҫ���·����ڴ�
        tstr->str = realloc(tstr->str, cap);
        tstr->cap = cap;
    }
    return tstr->str + tstr->len;
}

//
// tstr_popup - ���ַ���ͷ����len�����ַ�
// tstr		: �ɱ��ַ���
// len		: �����ĳ���
// return	: void
//
inline void 
tstr_popup(tstr_t tstr, size_t len) {
    if (len > tstr->len)
        tstr->len = 0;
    else {
        tstr->len -= len;
        memmove(tstr->str, tstr->str + len, tstr->len);
    }
}

//
// tstr_dupstr - �õ�һ�������c�Ĵ�, ��Ҫ�Լ��º�free
// tstr		: tstr_t ��
// return	: ���ش����õ�c��
//
char * 
tstr_dupstr(tstr_t tstr) {
    if (tstr && tstr->len >= 1) {
        // �����ڴ�, �������ս��
        size_t len = tstr->len + !!tstr->str[tstr->len - 1];
        char * str = malloc(len * sizeof(char));
        memcpy(str, tstr->str, len - 1);
        str[len - 1] = '\0';
        return str;
    }
    return NULL;
}

//
// tstr_t ��������, �����c��tstr������һ��tstr_t�ṹ���ַ���
// str		: ���������ַ���
// len		: �������ĳ���
// return	: ���ش����õ��ַ���,�ڴ治����ӡ��־�˳�����
//
tstr_t 
tstr_create(const char * str, size_t len) {
    return NULL;
}

tstr_t 
tstr_creates(const char * str) {
    return NULL;
}

//
// tstr_delete - tstr_t �ͷź���
// tstr		: ���ͷŵĴ��ṹ
// return   : void
//
void 
tstr_delete(tstr_t tstr) {

}

//
// ��tstr_t���ṹ������ַ���, �ڴ����ʧ���ڲ����Լ�����
// c		: ������ӵ�char
// str		: ��ӵ�c��
// sz		: ��Ӵ��ĳ���
//
void 
tstr_appendc(tstr_t tstr, int c) {

}
void 
tstr_appends(tstr_t tstr, const char * str) {

}
void 
tstr_appendn(tstr_t tstr, const char * str, size_t sz) {

}

//
// tstr_cstr - ͨ��cstr_t���õ�һ��c�Ĵ���'\0'��β
// tstr		: tstr_t ��
// return	: ���ع����õ�c�Ĵ�, �ڴ��ַtstr->str
//
char * 
tstr_cstr(tstr_t tstr) {
    if (tstr->len < 1u || tstr->str[tstr->len - 1]) {
        tstr_expand(tstr, 1u);
        tstr->str[tstr->len] = '\0';
    }
    return tstr->str;
}