#include    "serialize/UASDK_i16array.h"
#include    <errno.h>
#include    <memory.h>

static const uint8_t _8cc[] = UASDK_i16array_8cc;

int UASDK_i16array_new(uint32_t src_count, const int16_t* src_data, pUASDK_i16array_t* pparray)
{
    int err = EXIT_SUCCESS;
    do {
        BL2u16_t size = { sizeof(int16_t), src_count };
        BLarray1D_alloc(*pparray, size);
        if (*pparray == NULL)
        {
            err = ENOMEM;
            break;
        }
        memcpy(BLarray1D_begin(*pparray), src_data, (size[0] * size[1]));
    } while (0);
    return err;
}

int UASDK_i16array_serialize(const void* src, void* dst)
{
    pcUASDK_i16array_t p = (pcUASDK_i16array_t)src;
    uint8_t* pdst = (uint8_t*)dst;
    memcpy(pdst, _8cc, sizeof(_8cc));
    pdst += sizeof(_8cc);
    *(uint16_t*)pdst = p->size[0] * p->size[1];
    pdst += sizeof(uint16_t);
    memcpy(pdst, BLarray1D_begin(p), (p->size[0] * p->size[1]));
    return EXIT_SUCCESS;
}

int UASDK_i16array_serialize_estimate(const void* src)
{
    pcUASDK_i16array_t p = (pcUASDK_i16array_t)src;
    return  sizeof(_8cc) + 2 + (p->size[0] * p->size[1]);
}

int UASDK_i16array_deserialize(uint16_t src_byte_count, const void* src, void* dst)
{
    const uint8_t* p = (const uint8_t*)src;
    const uint16_t* psrc_size = (const uint16_t*)(p + 8);
    pUASDK_i16array_t pdst = (pUASDK_i16array_t)dst;
    pdst->size[0] = sizeof(int16_t);
    pdst->size[1] = *psrc_size / pdst->size[0];
    memcpy(BLarray1D_begin(pdst), (void*)(psrc_size + 1), *psrc_size);
    return EXIT_SUCCESS;
}

int UASDK_i16array_deserialize_estimate(uint16_t src_byte_count, const void* src)
{
    const uint8_t* p = (const uint8_t*)src;
    const uint16_t* psrc_size = (const uint16_t*)(p + 8);
    return  sizeof(BLarray1D_t) + *psrc_size;
}
