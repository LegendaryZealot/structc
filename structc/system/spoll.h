#ifndef _H_SPOLL
#define _H_SPOLL

#include <socket.h>

//
// ����������¼����ṹ
// event[INT_EVENT] -> event_t
//
struct event {
    void * ud;
    bool read;
    bool write;
    bool error;
};

#define MAX_EVENT (64)

typedef struct event event_t[MAX_EVENT];

#ifdef _MSC_VER
    typedef struct poll * poll_t;
#else
    typedef int poll_t;
#endif

//
// s_create     - ����һ�� poll ģ��
// s_error      - true ��ʾ��ǰ������ poll ģ��������
// s_delete     - ����һ�������� poll ģ��
//
extern poll_t s_create(void);
extern bool s_error(poll_t p);
extern void s_delete(poll_t p);

//
// s_del        - ɾ��1���� socket
// s_add        - ��Ӽ��� socket, �����ö�ģʽ, ʧ�ܷ��� true
// s_write      - �޸ĵ�ǰ socket, ͨ�� enable = true ����дģʽ
//
extern void s_del(poll_t p, socket_t s);
extern bool s_add(poll_t p, socket_t s, void * ud);
extern void s_write(poll_t p, socket_t s, void * ud, bool enable);

//
// s_wait       - wait ����, �ȴ�������Ͷ����
// p        : poll ģ��
// e        : ���صĲ����¼���
// return   : ���ش������¼�����, < 0 ��ʾʧ��
//
extern int s_wait(poll_t p, event_t e);

#endif//_H_SPOLL
