#include <dos.h>
#include <chead.h>

//
// main_run - ҵ�����е�����
// ͨ�� ISDEBUG -> main_test
//
void main_run(int argc, char * argv[]) {
#ifdef ISDEBUG
    TEST_FUNC(main_test);
#endif
    
    //
    // ��ʼ��ı��� ... 
    //
}