#include <uv.h>
#include <chead.h>
#include <assext.h>

struct love {
    uv_timer_t tick;

    uv_tty_t tty;

    int width;
    int height;
    int pos;

    char ** msgs;
    int len;
};

static char * _figure[] = {
    u8"  ��Ӱ :- ������\n",
    u8"  \n",
    u8"  ��Ӱ\n",
    u8"  ���Ǻܼ�\n",
    u8"  ��\n",
    u8"  ��һ�ַ羰\n",
    u8"  \n",
    u8"  ��Ӱ\n",
    u8"  ���Ǻ�����\n",
    u8"  ����\n",
    u8"  ��һ������\n",
    u8"  \n",
    u8"  ��Ӱ\n",
    u8"  ���Ǻܺ���\n",
    u8"  ����\n",
    u8"  ��һ������\n",
    u8"  \n",
    u8"  ��Ӱ\n",
    u8"  ���Ǻܹ���\n",
    u8"  ����\n",
    u8"  �����˼ǵ���\n"
};

// _love_stty : �ڲ�������Ϣ
static inline _love_stty(struct love * love, const char * msg) {
    uv_buf_t buf;
    buf.base = (char *)msg;
    buf.len = (int)strlen(buf.base);
    uv_try_write((uv_stream_t *)&love->tty, &buf, 1);
}

// _love_init : ��ʼ����ǰ tty �ṹ
static void _love_init(struct love * love) {
    uv_loop_t * loop = uv_default_loop();
    memset(love, 0, sizeof *love);

    // ��ʼ�� tty ����
    uv_tty_init(loop, &love->tty, 1, 0);
    uv_tty_set_mode(&love->tty, UV_TTY_MODE_NORMAL);

    // ֻ�� tty �������
    if (uv_guess_handle(1) != UV_TTY)
        CERR_EXIT("uv_guess_handle(1) != UV_TTY!");

    // ��ȡ��ǰ��Ļ�����Ϣ
    if (uv_tty_get_winsize(&love->tty, &love->width, &love->height)) {
        uv_tty_reset_mode();
        CERR_EXIT("Could not get TTY information");
    }

    // ���þ�������
    love->msgs = _figure;
    love->len = LEN(_figure);

    // ��ʼ����ʱ��
    uv_timer_init(loop, &love->tick);
}

// _love_screem : ��Ļ��������
static void _love_screem(struct love * love) {
    char buf[BUFSIZ];
    int cnt = love->pos < love->len ? love->pos : love->len;

    // ��������λ��
    int idx = love->height - love->pos;
    snprintf(buf, LEN(buf), "\033[2J\033[%dB", idx);
    _love_stty(love, buf);

    // ȫ����ʾ
    for (idx = 0; idx < cnt; idx++)
        _love_stty(love, love->msgs[idx]);
}

// _update - ����ˢ���¼�
static void _love_update(struct love * love) {
    ++love->pos;

    // ��ʼ��������
    _love_screem(love);

    // ���н���ֱ�ӷ���
    if (love->pos >= love->height) {
        // ����tty
        uv_tty_reset_mode();
        uv_timer_stop(&love->tick);
    }
}

//
// uv_love_test - �黳 ~
//
void uv_love_test(void) {
    struct love love;
    _love_init(&love);

    // ��ʼ��ʼ��, ��ʱ��ˢ���¼�
    uv_timer_start(&love.tick, (uv_timer_cb)_love_update, 200, 200);

    // �¼���������
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
