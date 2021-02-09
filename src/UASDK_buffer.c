#include    "UASDK_buffer.h"
#include    "base/BLbase.h"
#include    <errno.h>
#include    <stdlib.h>
#include    <stdio.h>
#include    <memory.h>

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

UASDK_bufcpresult_t UASDK_buffer_copy(pUASDK_buffer_t dst, pUASDK_buffer_t src, uint8_t *byte_count)
{
    UASDK_bufcpresult_t result = UASDK_bufcpresult_error;
    do {
        if (!dst || !src || !byte_count) { break; }
        uint8_t dst_available = dst->caplen.byte_capacity - dst->caplen.byte_filled;
        uint8_t src_available = src->caplen.byte_filled;
        if ((dst_available < *byte_count) || (src_available < *byte_count))
        { // available data is less than *byte_count.
            if (src_available < dst_available)
            { // available data is limited by src
                result = UASDK_bufcpresult_lessbysrc;
                *byte_count = src_available;
            }
            else
            { // available data is limited by dst
                result = UASDK_bufcpresult_lessbydst;
                *byte_count = dst_available;
            }
        }
        else
        { // available data is greater than or equal to *byte_count.
            result = UASDK_bufcpresult_fullsuccess;
        }
        // execute copy
        dst->caplen.byte_filled +=
            UASDK_buffer_shiftout(UASDK_buffer_vacant_begin(dst), src, *byte_count);
    } while (0);
    return result;
}

uint8_t UASDK_buffer_shiftout(void* dst, pUASDK_buffer_t src, uint8_t byte_count)
{
    if (src->caplen.byte_filled == 0) { return 0; }
    uint8_t shiftout_count = __min(byte_count, src->caplen.byte_filled);
    memcpy(dst, src->head.pbytes, shiftout_count);
    if (shiftout_count < src->caplen.byte_filled)
    {
        memcpy(src->head.pbytes, src->head.pbytes + shiftout_count, src->caplen.byte_filled - shiftout_count);
    }
    src->caplen.byte_filled -= shiftout_count;
    return shiftout_count;
}