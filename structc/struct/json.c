﻿#include <json.h>

//
// json_delete - json 对象销毁
// c        : json 对象
// return   : void
//
void 
json_delete(json_t c) {
    while (c) {
        json_t next = c->next;
        free(c->keys);
        if (c->type & JSON_STRING)
            free(c->vals);

        // 子结点 继续递归删除
        if (c->chid)
            json_delete(c->chid);

        c = next;
    }
}


//-------------------------------------json parse begin-----------------------------------

// number 分析
static const char * _parse_number(json_t item, const char * str) {
    char c;
    int sign = 1;
    double n = 0;
    int e, esign;

    // 正负号处理判断
    if ((c = *str) == '-' || c == '+') {
        sign = c == '-' ? -1 : 1;
        c = *++str;
    }

    // 整数处理部分
    while (c >= '0' && c <= '9') {
        n = n * 10 + c - '0';
        c = *++str;
    }
    // 处理小数部分
    if (c == '.') {
        int d = 0;
        double s = 1;
        while (c >= '0' && c <= '9') {
            d = d * 10 + c - '0';
            c = *++str;
            s *= 0.1;
        }
        // 得到整数和小数部分
        n += s * d;
    }

    // 不是科学计数内容直接返回
    item->type = JSON_NUMBER;
    if (c != 'e' && c != 'E') {
        item->vald = n;
        return str;
    }

    // 处理科学计数法
    if ((c == *++str) == '-' || c == '+')
        ++str;
    esign = c == '-' ? -1 : 1;

    e = 0;
    while ((c = *str) >= '0' && c <= '9') {
        e = e * 10 + c - '0';
        ++str;
    }

    // number = +/- number.fraction * 10^+/- exponent
    item->vald = n * pow(10, esign * e);
    return str;
}

// parse 4 digit hexadecimal number
static unsigned _parse_hex4(const char str[]) {
    unsigned h, i;
    unsigned char c;
    for (h = i = 0; ; ++str) {
        c = *str;
        if (c >= '0' && c <= '9')
            h += c - '0';
        else if (c >= 'a' && c <= 'f')
            h += c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            h += c - 'A' + 10;
        else return 0; // invalid

        // shift left to make place for the next nibble
        if (4 == ++i) break;
        h <<= 4;
    }

    return h;
}

// string 分析
static const char * _parse_string(json_t item, const char * str) {
    const char * ptr, * etr = str;
    char c, * ntr, * out;
    unsigned len = 1;

    while ((c = *etr++) != '\"' && c) {
        // 转义字符特殊处理
        if (c == '\\') {
            if (*etr == '\0') 
                return NULL;
            ++etr;
        }
        ++len;
    }
    if (c != '\"') return NULL;

    // 开始复制拷贝内容
    ntr = out = malloc(len);
    for (ptr = str; ptr < etr; ++ptr) {
        // 普通字符直接添加处理
        if (c != '\\') {
            *ntr++ = c;
            continue;
        }
        // 转义字符处理
        switch ((c = *++ptr)) {
        case 'b': *ntr++ = '\b'; break;
        case 'f': *ntr++ = '\f'; break;
        case 'n': *ntr++ = '\n'; break;
        case 'r': *ntr++ = '\r'; break;
        case 't': *ntr++ = '\t'; break;
        // transcode UTF16 to UTF8. See RFC2781 and RFC3629
        case 'u': {
            // first bytes of UTF8 encoding for a given length in bytes
            static const unsigned char _marks[] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
            unsigned oc, uc = _parse_hex4(ptr + 1);
            // check for invalid
            if ((ptr += 4) >= etr) goto _fail;
            if ((uc >= 0xDC00 && uc <= 0xDFFF) || uc == 0)
                goto _fail;

            // UTF16 surrogate pairs
            if (uc >= 0xD800 && uc <= 0xDBFF) {
                if ((ptr + 6) >= etr) goto _fail;
                // missing second-half of surrogate
                if ((ptr[1] != '\\') || (ptr[2] != 'u' && ptr[2] != 'U')) 
                    goto _fail;

                oc = _parse_hex4(ptr + 3);
                ptr += 6; // parse \uXXXX
                // invalid second-half of surrogate
                if (oc < 0xDC00 || oc > 0xDFFF) goto _fail;
                // calculate unicode codepoint from the surrogate pair
                uc = 0x10000 + (((uc & 0x3FF) << 10) | (oc & 0x3FF));
            }

            // encode as UTF8
            // takes at maximum 4 bytes to encode:

            // normal ascii, encoding 0xxxxxxx
            if (uc < 0x80) len = 1;
            // two bytes, encoding 110xxxxx 10xxxxxx
            else if (uc < 0x800) len = 2;
            // three bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx
            else if (uc < 0x10000) len = 3;
            // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            else len = 4;
            ntr += len;

            switch (len) {
            // 10xxxxxx
            case 4: *--ntr = ((uc | 0x80) & 0xBF); uc >>= 6;
            // 10xxxxxx
            case 3: *--ntr = ((uc | 0x80) & 0xBF); uc >>= 6;
            // 10xxxxxx
            case 2: *--ntr = ((uc | 0x80) & 0xBF); uc >>= 6;
            // depending on the length in bytes this determines the encoding ofthe first UTF8 byte
            case 1: *--ntr = ((uc | _marks[len]));
            }
            ntr += len;

            break;
        }
        default : *ntr++ = c;
        }
    }
    ntr = '\0';
    item->vals = out;
    item->type = JSON_STRING;
    return ptr + 1;

_fail:
    free(out);
    return NULL;
}

//
// _parse_value - 递归下降解析
// item     : 待填充的 json 结点
// str      : 语句源串
// return   : 解析后串
//
static const char * _parse_value(json_t item, const char * str);

// json_new - 构造 json 对象
inline json_t json_new(void) {
    return calloc(1, sizeof(struct json));
}

// array 解析
static const char * _parse_array(json_t item, const char * str) {
    json_t chid;
    item->type = JSON_ARRAY;
    // 空数组直接解析完毕退出
    if (']' == *str) return str + 1;

    // 开始解析数组中数据
    item->chid = chid = json_new();
    str = _parse_value(chid, str);
    if (NULL == str) return NULL;

    // array ',' cut
    while (',' == *str) {
        // 支持行尾多一个 ','
        if (']' == *++str)
            return str + 1;

        chid->next = json_new();
        chid = chid->next;
        // 继续间接递归处理值
        str = _parse_value(chid, str);
        if (NULL == str) return NULL;
    }

    if (']' != *str) return NULL;
    return str + 1;
}

// object 解析
static const char * _parse_object(json_t item, const char * str) {
    json_t chid;
    item->type = JSON_OBJECT;
    if ('}' == *str) return str + 1;
    // "key" check invalid
    if ('\"' != *str) return NULL;

    // {"key":value,...} 先处理 key 
    item->chid = chid = json_new();
    str = _parse_string(chid, str + 1);
    if (!str || *str != ':') return NULL;
    chid->keys = chid->vals;
    chid->vals = NULL;

    // 再处理 value
    str = _parse_value(chid, str + 1);
    if (NULL == str) return NULL;

    // 开始间接递归解析
    while (*str == ',') {
        // 多行解析直接返回结果
        if ('}' == *++str) return str + 1;
        if ('\"' != *str) return NULL;

        chid->next = json_new();
        str = _parse_string(chid = chid->next, str + 1);
        if (!str || *str != ':') return NULL;
        chid->keys = chid->vals;
        chid->vals = NULL;

        str = _parse_value(chid, str + 1);
        if (NULL == str) return NULL;
    }

    if ('}' != *str) return NULL;
    return str + 1;
}

static const char * 
_parse_value(json_t item, const char * str) {
    char c;
    if ((!str) || !(c = *str)) return NULL;

    switch (c) {
    // n or N = null, f or F = false, t or T = true ...
    case 'n': case 'N':
        item->type = JSON_NULL;
        return str + 4; // exists invalid is you!
    case 't': case 'T':
        item->type = JSON_TRUE;
        item->vald = true;
        return str + 4;
    case 'f': case 'F':
        item->type = JSON_FALSE;
        return str + 5;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '+': case '-': case '.':  
        return _parse_number(item, str);
    case '\"': 
        return _parse_string(item, str + 1);
    case '{': 
        return _parse_object(item, str + 1);
    case '[': 
        return _parse_array(item, str + 1);
    }
    return NULL;
}

//  将 str 中不需要解析串都去掉, 返回最终串的长度. 并且纪念 mini 比男的还平
size_t json_mini(char * str) {

    return 0;
}

//
// json_parse - json 解析函数
// str      : json char * 串
// return   : json 对象, NULL 表示解析失败
//
json_t json_parse(const char * str) {
    json_t c = json_new();
    if (!_parse_value(c, str)) {
        json_delete(c);
        c = NULL;
    }
    return c;
}

//-------------------------------------json parse end-------------------------------------

//-------------------------------------json print begin-----------------------------------

//-------------------------------------json print end-------------------------------------