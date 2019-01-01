#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)

#include <sys/event.h>

//
// s_create     - ���� poll ����
// s_error      - true ��ʾ���� poll �����쳣
// s_delete     - ���ٴ����� poll ����
//
inline poll_t s_create(void) {
    return epoll_create1(EPOLL_CLOEXEC);
}

inline bool s_error(poll_t p) {
    return p < 0;
}

inline void s_delete(poll_t p) {
    close(p);
}

//
// s_del        - ɾ������ soctt
// s_add        - ��Ӽ��� soctt, �����ö�ģʽ, ʧ�ܷ��� true
// s_write      - �޸ļ��� soctt, ͨ�� enable = true ����дģʽ
//
inline void s_del(poll_t p, socket_t s) {
    struct kevent t;
    EV_SET(&t, s, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(p, &t, 1, NULL, 0, NULL);
    EV_SET(&t, s, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(p, &t, 1, NULL, 0, NULL);
}

bool s_add(poll_t p, socket_t s, void * u) {
    struct kevent t;
    EV_SET(&t, s, EVFILT_READ, EV_ADD, 0, 0, u);
    if (kevent(p, &t, 1, NULL, 0, NULL) < 0 || t.flags & EV_ERROR)
        return true;

    EV_SET(&t, s, EVFILT_WRITE, EV_ADD, 0, 0, u);
    if (kevent(p, &t, 1, NULL, 0, NULL) < 0 || t.flags & EV_ERROR) {
        EV_SET(&t, s, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(p, &t, 1, NULL, 0, NULL);
        return true;
    }
    EV_SET(&t, s, EVFILT_WRITE, EV_DISABLE, 0, 0, u);
    if (kevent(p, &t, 1, NULL, 0, NULL) < 0 || t.flags & EV_ERROR) {
        s_del(p, sock);
        return 1;
    }
    return false;
}

inline void s_write(poll_t p, socket_t s, void * u, bool enable) {
    struct kevent t;
    EV_SET(&t, s, EVFILT_WRITE, enable ? EV_ENABLE : EV_DISABLE, 0, 0, u);
    if (kevent(p, &t, 1, NULL, 0, NULL) < 0 || t.flags & EV_ERROR)
        CERR("kevent error %p, %d, %d", u, enable, t.flags);
}

//
// s_wait       - wait ����, �������
// p        : poll ����
// e        : ���ز����¼���
// return   : ���ز����¼�����, < 0 ��ʾʧ��
//
int s_wait(poll_t p, event_t e) {
    struct kevent v[MAX_EVENT];
    int n = kevent(p, NULL, 0, v, sizeof v / sizeof *v, NULL);

    for (int i = 0; i < n; ++i) {
        unsigned filter = v[i].filter;
        bool eof = v[i].flags & EV_EOF;
        e[i].u = v[i].udata;
        e[i].write = filter == EVFILT_WRITE && !eof;
        e[i].read = filter == EVFILT_READ && !eof;
        e[i].error = v[i].flags & EV_ERROR;
        e[i].eof = eof;
    }

    return n;
}

#endif
