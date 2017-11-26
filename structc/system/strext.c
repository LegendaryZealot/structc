#include <strext.h>
#include <assext.h>
#include <ctype.h>

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
// str_trim - ȥ���ַ�����ǰ������ַ�
// str      : ���������ַ����� \0 ��β
// return   : ���ع������ַ������׵�ַ
//
char * 
str_trim(char str[]) {
    char * s, * e;
    if (!str || !*str)
        return str;

    // �ҵ���һ�����ǿո��ַ��ĵ�ַ
    for (s = str; isspace(*s); ++s)
        ;

    // �ҵ����һ�����ǿո��ַ��ĵ�ַ
    e = s + strlen(s) - 1;
    if (isspace(*e)) {
        do --e; while (isspace(*e));
        e[1] = '\0';
    }

    // ��ʼ�����ƶ�����׵�ַ
    return s == str ? str : memmove(str, s, e - s + 2);
}


// _str_printf : �ɹ�ֱ�ӷ���
static char * _str_printf(const char * format, va_list arg) {
    char buf[BUFSIZ];
    int len = vsnprintf(buf, sizeof buf, format, arg);
    assert(len >= 0);
    if (len < sizeof buf) {
        char * ret = malloc(len + 1);
        assert(ret != NULL);
        return memcpy(ret, buf, len + 1);
    }
    return NULL;
}

//
// str_printf - �ַ�����������
// format   : ������ʽ����pritnf
// ...      : ������
// return   : char * �����ڴ�
//
char * 
str_printf(const char * format, ...) {
    char * ret;
    int len, cap;
    va_list arg;
    va_start(arg, format);

    // BUFSIZ �����ڴ�ֱ�ӷ���
    ret = _str_printf(format, arg);
    if (ret != NULL)
        return ret;

    cap = BUFSIZ << 1;
    for (;;) {
        ret = malloc(cap);
        assert(ret != NULL);
        len = vsnprintf(ret, cap, format, arg);
        // ʧ�ܵ����
        if (len < 0) {
            free(ret);
            RETURN(NULL, "vsnprintf error format is = %s.", format);
        }

        // �ɹ����
        if (len < cap)
            break;

        // �ڴ治������
        free(ret);
        cap <<= 1;
    }

    return realloc(ret, len + 1);
}

//
// str_freadend - �򵥵��ļ���ȡ��,���ȡ�������ļ����ݷ���, ��Ҫ�Լ�free
// path		: �ļ�·��
// return	: �����õ��ַ�������, ����NULL��ʾ��ȡʧ��
//
char * 
str_freadend(const char * path) {
	int err;
	size_t rn, cap, len;
	char * str, buf[BUFSIZ];
	FILE * txt = fopen(path, "rb");
	if (NULL == txt) {
		RETURN(NULL, "fopen rb path error = %s.", path);
	}

	// �����ڴ�
	len = 0;
	str = malloc(cap = BUFSIZ);

	// ��ȡ�ļ�����
	do {
		rn = fread(buf, sizeof(char), BUFSIZ, txt);
		if ((err = ferror(txt))) {
			free(str);
			fclose(txt);
			RETURN(NULL, "fread err path = %d, %s.", err, path);
		}
		// ��ʼ��ӹ�������
		if (len + rn >= cap)
			str = realloc(str, cap <<= 1);
		memcpy(str + len, buf, rn);
		len += rn;
	} while (rn == BUFSIZ);

	// ���ý�β, �����ؽ��
	str[len] = '\0';
	return realloc(str, len + 1);
}

// _str_fwrite - ����Լ��������ݵ��ļ���
static int _str_fwrite(const char * path, const char * str, const char * mode) {
	int len;
	FILE * txt;
	if (!path || !*path || !str || !mode) {
		RETURN(EParam, "check !path || !*path || !str || !mode");
	}

	// ���ļ�, д����Ϣ, �ر��ļ�
	if ((txt = fopen(path, mode)) == NULL) {
		RETURN(EFd, "fopen error path = %s, mode = %s.", path, mode);
	}
	len = fputs(str, txt);
	fclose(txt);
	// ����ļ�����
	return len;
}

//
// str_fwrites - ��c��str����д�뵽path·�����ļ���
// path		: �ļ�·��
// str		: c�Ĵ�����
// return	: >=0 is success, < 0 is error
//
inline int 
str_fwrites(const char * path, const char * str) {
	return _str_fwrite(path, str, "wb");
}

//
// str_fappends - ��c��strд�뵽path·�����ļ���ĩβ
// path		: �ļ�·��
// str		: c�Ĵ�����
// return	: >=0 is success, < 0 is error
//
inline int 
str_fappends(const char * path, const char * str) {
	return _str_fwrite(path, str, "ab");
}
