#include <list.h>

//
// list_delete - �����������ٲ���
// list     : ����������ṹ
// return   : void
//
void 
list_delete(list_t list) {
    struct $list * head;
    if (!list || !list->fdie)
        return;

    // ��ϸ�����������ݱ仯
    head = list->root;
    while (head) {
        struct $list * next = head->next;
        list->fdie(head);
        head = next;
    }
}