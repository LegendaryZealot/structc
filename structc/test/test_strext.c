#include "../struct/strext.h"
#include "../chead.h"

/*
 ��Ԫ���ܲ���Ҫ�� : 
 
 ��д���� : 
 {�ӿ�}.h -> test_{�ӿ�}.c -> void test_{�ӿ�}(void) { ... }

 ���ù��� :
 chead.h :: TEST_FUNC(test_{�ӿ�}) ���� structc.c :: test_structc 

 */

// test_strext - ���� strext.h �нӿ����
void test_strext(void) {
    unsigned hash = str_hash(CSTR(__func__)); 
    printf("%s -> %u\n", CSTR(__func__), hash);
}