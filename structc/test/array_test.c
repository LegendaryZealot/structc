#include <array.h>

//
// array test
//
void array_test(void) {
    // ����һ����ջ�ϵĶ�̬����
    ARRAY_CREATE(double, as);

    // ��ʼ��������
    *(double *)array_push(as) = 1.1234;

    *(double *)array_push(as) = 2.2345;

    *(double *)array_push(as) = 4.9876;

    // �������
    printf("a = %lf\n", *(double *)array_pop(as));

    printf("a = %lf\n", *(double *)array_pop(as));

    printf("a = %lf\n", *(double *)array_pop(as));

    ARRAY_DELETE(as);
}