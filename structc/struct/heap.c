#include "heap.h"

#define UINT_HEAP       (1<<5u)

struct heap {
    cmp_f   fcmp;       // �Ƚ���Ϊ
    unsigned len;       // heap ����
    unsigned cap;       // heap ����
    void ** data;       // ���ݽڵ�����
};

// heap_expand - ��ӽڵ�����
inline void heap_expand(struct heap * h) {
    if (h->len >= h->cap) {
        h->data = realloc(h->data, h->cap<<=1);
        assert(h->data);
    }
}

//
// heap_create - �������Ϲ���Ķ�
// fcmp     : �Ƚ���Ϊ, ���� fcmp() <= 0
// return   : ���ش����õĶѶ���
//
inline heap_t 
heap_create(cmp_f fcmp) {
    struct heap * h = malloc(sizeof(struct heap));
    assert(h && fcmp);
    h->fcmp = fcmp;
    h->len = 0;
    h->cap = UINT_HEAP;
    h->data = malloc(sizeof(void *) * UINT_HEAP);
    assert(h->data && UINT_HEAP > 0);
    return h;
}

//
// heap_delete - ���ٶ�
// h        : �Ѷ���
// fdie     : ������Ϊ, Ĭ�� NULL
// return   : void
//
void 
heap_delete(heap_t h, node_f fdie) {
    if (NULL == h || h->data == NULL) return;
    if (fdie && h->len > 0)
        for (unsigned i = 0; i < h->len; ++i)
            fdie(h->data[i]);
    free(h->data);
    h->data = NULL;
    h->len = 0;
    free(h);
}

// down - �ѽڵ��³�, ���ϵ��³�һ��
static void down(cmp_f fcmp, void * data[], unsigned len, unsigned x) {
    void * m = data[x];
    for (unsigned i = x * 2 + 1; i < len; i = x * 2 + 1) {
        if (i + 1 < len && fcmp(data[i+1], data[i]) < 0)
            ++i;
        if (fcmp(m, data[i]) <= 0)
            break;
        data[x] = data[i];
        x = i;
    }
    data[x] = m;
}

// up - �ѽڵ��ϸ�, ���µ��ϸ�һ��
static void up(cmp_f fcmp, void * node, void * data[], unsigned x) {
    while (x > 0) {
        void * m = data[(x-1)>>1];
        if (fcmp(m, node) <= 0)
            break;
        data[x] = m;
        x = (x-1)>>1;
    }
    data[x] = node;
}

//
// heap_insert - �Ѳ�������
// h        : �Ѷ���
// node     : ��������
// return   : void
//
inline void 
heap_insert(heap_t h, void * node) {
    heap_expand(h);
    up(h->fcmp, node, h->data, h->len++);
}

//
// heap_remove - ��ɾ������
// h        : �Ѷ���
// arg      : ��������
// fcmp     : �Ƚ���Ϊ, ���� fcmp() == 0
// return   : �ҵ��Ķѽڵ�
//
void * 
heap_remove(heap_t h, void * arg, cmp_f fcmp) {
    if (h == NULL || h->len <= 0)
        return NULL;

    // ��ʼ��������ڵ�
    unsigned i = 0;
    fcmp = fcmp ? fcmp : h->fcmp;
    do {
        void * node = h->data[i];
        if (fcmp(arg, node) == 0) {
            // �ҵ��ڵ㿪ʼ��ɾ������
            if (--h->len > 0 && h->len != i) {
                // β�ͽڵ�ʹ�ɾ���ڵ�Ƚ�
                int ret = h->fcmp(h->data[h->len], node);

                // С����, �µ�ֵ���ϵ�ֵС, ��ô�ϸ�
                if (ret < 0)
                    up(h->fcmp, h->data[h->len], h->data, i);
                else if (ret > 0) {
                    // С����, �µ�ֵ���ϵ�ֵ��, ��ô�³�
                    h->data[i] = h->data[h->len];
                    down(h->fcmp, h->data, h->len, i);
                }
            }

            return node;
        }
    } while (++i < h->len);

    return NULL;
}

//
// heap_top - �鿴�Ѷ��ڵ�����
// h        : �Ѷ���
// return   : �Ѷ��ڵ�
//
inline void * 
heap_top(heap_t h) {
    return h->len <= 0 ? NULL : *h->data;
}

//
// heap_top - ժ���Ѷ��ڵ�����
// h        : �Ѷ���
// return   : ���ضѶ��ڵ�
//
inline void * 
heap_pop(heap_t h) {
    void * node = heap_top(h);
    if (node && --h->len > 0) {
        // β�ͽڵ�һ����С�Ѷ��ڵ��, ��ôҪ�³�
        h->data[0] = h->data[h->len];
        down(h->fcmp, h->data, h->len, 0);
    }
    return node;
}
