#include <sdos.h>
#include <chead.h>

//
// main_run - ҵ�����е�����
// ͨ�� _DEBUG -> main_test
//
void main_run(int argc, char * argv[]) {
#ifdef _DEBUG
    TEST_FUNC(main_test);
#endif
    
    //
    // ��ʼ��ı��� ... 
    //

}
