#include "strext.h"
#include "chead.h"

/*
 ��Ԫ���ܲ���Ҫ�� : 
 
 ��д���� : 
 {�ӿ�}.h -> {�ӿ�}_test.c -> void {�ӿ�}_test(void) { ... }

 ���ù��� :
 chead.h :: TEST_FUNC({�ӿ�}_test) ���� structc.c :: test_structc 

 */

//
// strext_test - ���� strext.h �нӿ����
//
void strext_test(void) {
    unsigned hash = str_hash(CSTR(__func__)); 
    printf("%s -> %u\n", CSTR(__func__), hash);
}
