#include    "UASDK_buffer.h"
#include    <errno.h>
#include    <stdlib.h>
#include    <stdio.h>

int UASDK_buffer_new(uint8_t capacity, pUASDK_buffer_t *ppbuffer)
{
    int err = EXIT_SUCCESS;
    do {
        size_t cb_head = sizeof(UASDK_buffer_t);
        size_t cb_data = capacity;
        size_t cb_alloc = capacity + cb_head;
        pUASDK_buffer_t p = (*ppbuffer = (pUASDK_buffer_t)malloc(cb_alloc));
        if (!p)
        {
            fprintf(stderr, "%s,%d,err=%d(0x%04x)\n", __FILE__, __LINE__, err, err);
            err = ENOMEM;
            break;
        }
        p->head.pbytes = (uint8_t*)(p->buf);
        p->caplen.byte_capacity = capacity;
        p->caplen.byte_filled = 0;
    } while (0);
    return err;
}

void UASDK_buffer_clear(pUASDK_buffer_t buffer)
{
    buffer->caplen.byte_filled = 0;
    *(buffer->head.pstring) = '\0';
}