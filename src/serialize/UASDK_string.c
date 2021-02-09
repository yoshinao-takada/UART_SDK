#include    "serialize/UASDK_string.h"
#include    "base/BLsv.h"
#include    <errno.h>
#include    <string.h>

static const uint8_t _8cc[] = UASDK_string_8cc;

int UASDK_string_new(const char* src_string, pUASDK_string_t* ppstring)
{
    int err = EXIT_SUCCESS;
    do {
        BL2u16_t array_size = { 1, (uint16_t)(strlen(src_string) + 1) };
        BLarray1D_alloc(*ppstring, array_size);
        if (*ppstring == NULL)
        {
            err = ENOMEM;
            break;
        }
        strcpy(BLarray1D_begin(*ppstring), src_string);
    } while (0);
    return err;
}

int UASDK_string_new2(uint32_t src_char_count, const char* src_string, pUASDK_string_t* ppstring)
{
    int err = EXIT_SUCCESS;
    do {
        BL2u16_t array_size = { 1, (uint16_t)src_char_count };
        BLarray1D_alloc(*ppstring, array_size);
        if (*ppstring == NULL)
        {
            err = ENOMEM;
            break;
        }
        memcpy(BLarray1D_begin(*ppstring), src_string, src_char_count);
    } while (0);
    return err;
}

int UASDK_string_serialize(const void* src, void* dst)
{
    int err = EXIT_SUCCESS;
    pcBLarray1D_t psrc = (pcBLarray1D_t)src;
    uint8_t* pdst = (uint8_t*)dst;
    do {
        memcpy(pdst, _8cc, sizeof(_8cc));
        pdst += sizeof(_8cc);
        *(uint16_t*)pdst = psrc->size[1];
        pdst += sizeof(uint16_t);
        memcpy(pdst, BLarray1D_begin(psrc), (size_t)psrc->size[1]);
    } while (0);
    return err;
}

int UASDK_string_serialize_estimate(const void* src)
{
    pcBLarray1D_t p = (pcBLarray1D_t)src;
    return
        8 + // 8CC code
        2 + // data length
        (int)p->size[1]; // string length
}

int UASDK_string_deserialize(uint16_t src_byte_count, const void* src, void* dst)
{
    int err = EXIT_SUCCESS;
    const uint8_t* psrc = (const uint8_t*)src;
    pBLarray1D_t pdst = (pBLarray1D_t)dst;
    do {
        psrc += sizeof(_8cc);
        pdst->size[0] = 1;
        pdst->size[1] = *(uint16_t*)psrc;
        psrc += sizeof(pdst->size[1]);
        memcpy(BLarray1D_begin(pdst), psrc, pdst->size[1]);
    } while (0);
    return err;
}

int UASDK_string_deserialize_estimate(uint16_t src_byte_count, const void* src)
{
    int result = -1;
    const uint8_t* psrc = (const uint8_t*)src;
    do {
        // check 8cc code
        if (!BLeq8(_8cc, psrc))
        {
            break;
        }
        // get pBLarray1D_t.size
        BL2u16_t size = { 1, *(uint16_t*)(psrc + sizeof(_8cc)) };
        // calculate result
        uint16_t src_byte_count_minimum = sizeof(_8cc) + sizeof(size[1]) + size[1];
        if (src_byte_count < src_byte_count_minimum)
        {
            break;
        }
        result = sizeof(size) + size[1];
    } while (0);
    return result;
}