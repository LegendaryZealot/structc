#include <chead.h>
#include <strext.h>

/*
 ��Ԫ���ܲ���Ҫ�� : 
 
 ��д���� : 
 {�ӿ�}.h -> {�ӿ�}_test.c -> void {�ӿ�}_test(void) { ... }

 ���ù��� :
 chead.h :: TEST_FUNC({�ӿ�}_test) ���� structc.c :: test_structc 

 */

//
// estring_test - ���� estring_test.h �нӿ����
//
void strext_test(void) {
    unsigned hash = str_hash(STR(__func__)); 
    printf("%s -> %u\n", STR(__func__), hash);

	// ������ݲ���
	char * txt = str_freadend("README.md");
	puts(txt);
}
