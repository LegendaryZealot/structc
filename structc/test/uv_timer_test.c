#include <uv.h>
#include <assext.h>

// �̳� uv_timer_t �ṹ
struct gravity {
    uv_timer_t tick;

    uv_tty_t tty;

    int width;
    int height;
    int pos;

    char * msg;
};

// _update - ����ͼƬ����
static void _update(struct gravity * grav) {
    char data[BUFSIZ];
    uv_buf_t buf;
    buf.base = data;
    //
    // \033[2J      : ����
    // \033[H       : ����Ƶ�(0, 0)
    // \033[%dB     : ������� %d ��
    // \033[%dC     : ������� %d ��
    // \033[42;37m  : ��ɫ 41 �̵�, ��ɫ 37 ����
    //
    // \033[0m      : �ر���������
    //
    buf.len = sprintf(data, "\033[2J\033[H\033[%dB\033[%dC\033[42;37m%s",
                            grav->pos,
                            (grav->width - (int)strlen(grav->msg)) / 2,
                            grav->msg);
    assert(buf.len < BUFSIZ);
    if (grav->pos == grav->height) {
        // �ر���Ļ��������
        const char * resets = "\033[0m";
        strcat(data, resets);
        buf.len += (int)strlen(resets);
    }

    // д����Ϣ
    uv_try_write((uv_stream_t *)&grav->tty, &buf, 1);

    // ��������ǰ��Ļ, �˳���ʱ��
    if (++grav->pos > grav->height) {
        // ����tty
        uv_tty_reset_mode();
        uv_timer_stop(&grav->tick);
    }
}

//
// uv_timer_test - ���� timer ʹ��
//
void uv_timer_test(void) {
    uv_loop_t * loop = uv_default_loop();
    struct gravity grav = { { 0 } };

    uv_tty_init(loop, &grav.tty, 1, 0);
    uv_tty_set_mode(&grav.tty, UV_TTY_MODE_NORMAL);

    // ��ȡ��ǰ��Ļ�����Ϣ
    if (uv_tty_get_winsize(&grav.tty, &grav.width, &grav.height)) {
        fprintf(stderr, "Could not get TTY information\n");
        uv_tty_reset_mode();
        return;
    }

    fprintf(stderr, "Width %d, height %d\n", grav.width, grav.height);
    
    // ���� timer ˢ����Ļ��Ϣ
    grav.msg = u8"�Ҳ����� ~";
    uv_timer_init(loop, &grav.tick);
    uv_timer_start(&grav.tick, (uv_timer_cb)_update, 200, 200);
    
    uv_run(loop, UV_RUN_DEFAULT);
}