#include    "base/BLringbuf.h"
#include    "base/BLbase.h"
#include    <errno.h>
#include    <assert.h>

#pragma region ring_buffer_without_exclusive_control
int BLringbuf_new(uint8_t count_bits, pBLringbuf_t* ppobj)
{
    int err = EXIT_SUCCESS;
    do {
        if ((count_bits == 0) || (count_bits > 16))
        {
            err = EINVAL;
            break;
        }
        size_t data_buffer_byte_count = (1 << count_bits);
        size_t head_byte_count = sizeof(BLringbuf_t);
        size_t alloc_byte_count = data_buffer_byte_count + head_byte_count;
        pBLringbuf_t p = (*ppobj = (pBLringbuf_t)malloc(alloc_byte_count));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->mask = (uint16_t)(data_buffer_byte_count - 1);
        p->wrptr = p->rdptr = 0;
    } while (0);
    return err;
}

int BLringbuf_put(pBLringbuf_t obj, uint16_t byte_count, const uint8_t* data, uint16_t* actual)
{
    int err = EXIT_SUCCESS;
    do {
        uint16_t available_space = BLringbuf_available_space(obj);
        if (available_space < byte_count)
        {
            *actual = available_space;
            err = ENOBUFS;
        }
        else
        {
            *actual = byte_count;
        }
        uint16_t actual_copy_byte_count = *actual;
        for (uint16_t i = 0; i < actual_copy_byte_count; i++)
        {
            obj->buffer[obj->wrptr] = data[i];
            obj->wrptr = (obj->wrptr + 1) & obj->mask;
        }
    } while (0);
    return err;
}

int BLringbuf_putforce(pBLringbuf_t obj, uint16_t byte_count, const uint8_t* data)
{
    int err = EXIT_SUCCESS;
    do {
        uint16_t discard_bytes = 0;
        uint16_t available_space = BLringbuf_available_space(obj);
        if (byte_count > available_space)
        {
            err = ENOBUFS;
            discard_bytes = byte_count - available_space;
        }
        for (uint16_t i = 0; i < byte_count; i++)
        {
            if (i < discard_bytes)
            {
                obj->rdptr = (obj->rdptr + 1) & obj->mask;
            }
            obj->buffer[obj->wrptr] = data[i];
            obj->wrptr = (obj->wrptr + 1) & obj->mask;
        }
    } while (0);
    return err;
}

int BLringbuf_get(pBLringbuf_t obj, uint16_t byte_count, uint8_t* data, uint16_t* actual)
{
    int err = EXIT_SUCCESS;
    do {
        uint16_t available_data = BLringbuf_available_data(obj);
        if (available_data < byte_count)
        {
            *actual = available_data;
            err = ENODATA;
        }
        else
        {
            *actual = byte_count;
        }
        uint16_t actual_copy_byte_count = *actual;
        for (uint16_t i = 0; i < actual_copy_byte_count; i++)
        {
            data[i] = obj->buffer[obj->rdptr];
            obj->rdptr = (obj->rdptr + 1) & obj->mask;
        }
    } while (0);
    return err;
}

int BLringbuf_peek(pBLringbuf_t obj, uint16_t byte_count, uint8_t* data, uint16_t* actual)
{
    int err = EXIT_SUCCESS;
    do {
        uint16_t available_data = BLringbuf_available_data(obj);
        if (available_data < byte_count)
        {
            *actual = available_data;
            err = ENODATA;
        }
        else
        {
            *actual = byte_count;
        }
        uint16_t actual_copy_byte_count = *actual;
        uint16_t peekptr = obj->rdptr;
        for (uint16_t i = 0; i < actual_copy_byte_count; i++)
        {
            data[i] = obj->buffer[peekptr];
            peekptr = (peekptr + 1) & obj->mask;
        }
    } while (0);
    return err;
}

void BLringbuf_wrptr_goahead(pBLringbuf_t obj, uint16_t offset)
{
    for (uint16_t i = 0; i < offset; i++)
    {
        obj->wrptr = (obj->wrptr + 1) & obj->mask;
    }
}

void BLringbuf_rdptr_goahead(pBLringbuf_t obj, uint16_t offset)
{
    for (uint16_t i = 0; i < offset; i++)
    {
        obj->rdptr = (obj->rdptr + 1) & obj->mask;
    }
}


uint16_t BLringbuf_available_space(pcBLringbuf_t obj)
{
    uint16_t diff = (obj->rdptr - obj->wrptr - 1);
    if (diff & 0x8000)
    {
        diff &= obj->mask;
        diff += (obj->mask + 1);
        diff &= obj->mask;
    }
    return diff; 
}

uint16_t BLringbuf_available_data(pcBLringbuf_t obj)
{
    uint16_t diff = (obj->wrptr - obj->rdptr);
    if (diff & 0x8000)
    {
        diff &= obj->mask;
        diff += (obj->mask + 1);
        diff &= obj->mask;
    }
    return diff;
}

void BLringbuf_clear(pBLringbuf_t obj)
{
    obj->rdptr = obj->wrptr = 0;
}

int BLringbuf_putptr(pBLringbuf_t obj, void* pv)
{
    int err = EXIT_SUCCESS;
    uint16_t transfer_size = sizeof(pv);
    do {
        uint16_t available_space = BLringbuf_available_space(obj);
        if (available_space < transfer_size)
        {
            err = ENOSPC;
            break;
        }
        BLringbuf_put(obj, transfer_size, (const uint8_t*)&pv, &transfer_size);
    } while (0);
    return err;
}

void* BLringbuf_getptr(pBLringbuf_t obj)
{
    void* pv = NULL;
    uint16_t transfer_size = (uint16_t)sizeof(pv);
    if (BLringbuf_available_data(obj) >= transfer_size)
    {
        BLringbuf_get(obj, transfer_size, (uint8_t*)&pv, &transfer_size);
    }
    return pv;
}

void BLringbuf_clearptr(pBLringbuf_t obj)
{
    for (void* pv = BLringbuf_getptr(obj); pv; pv = BLringbuf_getptr(obj))
    {
        free(pv);
    }
}

uint8_t BLringbuf_sizeindex(uint16_t count, uint16_t element_size)
{
    uint32_t byte_count = (uint32_t)count * (uint32_t)element_size;
    uint32_t buffer_size = 1;
    uint16_t index = 0;
    do {
        if (byte_count >= 0x10000)
        {
            break;
        }
        while ((buffer_size - 1) < byte_count)
        {
            buffer_size = (buffer_size << 1);
            index++;
        }
        if (buffer_size >= 0x10000)
        {
            index = 0;
        }
    } while (0);
    return index;
}
#pragma endregion ring_buffer_without_exclusive_control
#pragma region ring_buffer_with_exclusive_control
int BLringbufex_new(uint8_t count_bits, pBLringbufex_t* ppobj)
{
    assert(*ppobj == NULL);
    int err = EXIT_SUCCESS;
    do {
        if ((count_bits == 0) || (count_bits > 16))
        {
            err = EINVAL;
            break;
        }
        size_t data_buffer_byte_count = (1 << count_bits);
        size_t head_byte_count = sizeof(BLringbufex_t);
        size_t alloc_byte_count = data_buffer_byte_count + head_byte_count;
        pBLringbufex_t p = (*ppobj = (pBLringbufex_t)malloc(alloc_byte_count));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->buf.mask = (uint16_t)(data_buffer_byte_count - 1);
        p->buf.wrptr = p->buf.rdptr = 0;
        err = pthread_mutex_init(&p->mutex, (const pthread_mutexattr_t*)NULL);
    } while (0);
    return err;
}

int BLringbufex_newptrqueue(uint16_t least_count, pBLringbufex_t* ppobj)
{
    int err = EXIT_SUCCESS;
    do {
        uint8_t count_bits = 0;
        uint32_t buffer_size = 1;
        uint32_t least_size = sizeof(void*) * least_count;
        if (least_size >= 0xffff)
        {
            err = ERANGE;
            break;
        }
        while (buffer_size <= least_size)
        {
            count_bits++;
            buffer_size = (buffer_size << 1);
        }
        err = BLringbufex_new(count_bits, ppobj);
    } while (0);
    return err;
}

void BLringbufex_delete(pBLringbufex_t *ppobj)
{
    assert(ppobj && *ppobj);
    pBLringbufex_t p = *ppobj;
    pthread_mutex_destroy(&p->mutex);
    BLSAFEFREE(ppobj);
}

void BLringbufex_deleteptrqueue(pBLringbufex_t* ppobj)
{
    assert(ppobj && *ppobj);
    pBLringbufex_t p = *ppobj;
    BLringbuf_clearptr(&p->buf); // clear contents, freeing content pointers.
    BLringbufex_delete(ppobj); // delete the ringbuffer and the mutex.
}

int BLringbufex_lock(pBLringbufex_t obj)
{
    return pthread_mutex_lock(&obj->mutex);
}

int BLringbufex_unlock(pBLringbufex_t obj)
{
    return pthread_mutex_unlock(&obj->mutex);
}

int BLringbufex_putptr(pBLringbufex_t obj, void* pv)
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = BLringbufex_lock(obj)))
        {
            break;
        }
        err = BLringbuf_putptr(&obj->buf, pv);
        BLringbufex_unlock(obj);
    } while (0);
    return err;
}


void* BLringbufex_getptr(pBLringbufex_t obj)
{
    void* pv = NULL;
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = BLringbufex_lock(obj)))
        {
            break;
        }
        pv = BLringbuf_getptr(&obj->buf);
        BLringbufex_unlock(obj);
    } while (0);
    return pv;
}
#pragma endregion ring_buffer_with_exclusive_control
