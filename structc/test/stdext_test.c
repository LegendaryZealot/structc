#include <stdext.h>

//
// �򵥲���һ���ڴ�ģ��ʹ��
//
void stdext_test(void) {
    int * piyo = malloc(sizeof(int));
    printf("piyo = %p, %d.\n", piyo, *piyo);
    free(piyo);

    piyo = calloc(2, sizeof (int));
    printf("piyo = %p, %d.\n", piyo, *piyo);

    piyo = realloc(piyo, sizeof(int));
    printf("piyo = %p, %d.\n", piyo, *piyo);
    free(piyo);
}
