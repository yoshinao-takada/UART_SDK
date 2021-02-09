#include    "serialize/UASDK_r32array.h"
#include    "base/BLbase.h"


int UT_r32array_new();
int UT_r32array_serialize_deserialize();

int test_UASDK_r32array()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = UT_r32array_new())) { break; }
        if (EXIT_SUCCESS != (err = UT_r32array_serialize_deserialize())) { break; }
    } while (0);
    return err;
}


int UT_r32array_new()
{
    int err = EXIT_SUCCESS;
    pUASDK_r32array_t array[1024];
    BL1r32_t src_raw_array[4096];
    do {
        srand(1);
        for (int i = 0; i < ARRAYSIZE(array); i++)
        {
            for (int j = 0; j < ARRAYSIZE(src_raw_array); j++)
            {
                src_raw_array[j] = (BL1r32_t)rand();
            }
            UASDK_r32array_new(ARRAYSIZE(src_raw_array), src_raw_array, &array[i]);
        }
        srand(1);
        for (int i = 0; i < ARRAYSIZE(array); i++)
        {
            const BL1r32_t* p = (const BL1r32_t*)BLarray1D_begin(array[i]);
            for (uint16_t j = 0; j < array[i]->size[1]; j++)
            {
                if (p[j] != (BL1r32_t)rand())
                {
                    err = EXIT_FAILURE;
                    break;
                }
            }
            if (err) break;
        }
    } while (0);
    UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
    return err;
}

int UT_r32array_serialize_deserialize()
{
    static const uint8_t _8cc[] = UASDK_r32array_8cc;
    static const BL1r32_t src_raw_array[] = { -1.0f, 0.7f, 0.55f, 0.3f, 1.5f, -2.0f };
    int err = EXIT_SUCCESS;
    uint8_t* byte_buffer = NULL;
    pUASDK_r32array_t restored = NULL;
    pUASDK_r32array_t src = NULL;
    do {
        if (EXIT_SUCCESS != (err = UASDK_r32array_new(ARRAYSIZE(src_raw_array), src_raw_array, &src)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        int serialized_byte_count = UASDK_r32array_serialize_estimate(src);
        byte_buffer = (uint8_t*)malloc(serialized_byte_count + 4);
        uint8_t check_data[] = { 0x55, 0x83, 0xcc, 0xaa };
        BLcopy4(byte_buffer + serialized_byte_count, check_data);
        if (EXIT_SUCCESS != (err = UASDK_r32array_serialize(src, (void*)byte_buffer)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        // check check_data to confirm no pointer overrun
        if (!BLeq4(check_data, byte_buffer + serialized_byte_count))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        // check serialized data
        if (!BLeq8(_8cc, byte_buffer))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (sizeof(src_raw_array) != *(uint16_t*)(byte_buffer + 8))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        const BL1r32_t* i_src = (const BL1r32_t*)BLarray1D_begin(src);
        const BL1r32_t* i_serialized = (const BL1r32_t*)(byte_buffer + 10);
        for (uint32_t i = 0; i < ARRAYSIZE(src_raw_array); i++)
        {
            if (i_src[i] != i_serialized[i])
            {
                err = EXIT_FAILURE;
                break;
            }
        }
        if (err)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        // deserialize
        int deserialized_byte_count = UASDK_r32array_deserialize_estimate((uint16_t)serialized_byte_count, byte_buffer);
        restored = (pUASDK_r32array_t)malloc(deserialized_byte_count);
        if (!restored)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (EXIT_SUCCESS != (err = UASDK_r32array_deserialize(serialized_byte_count, byte_buffer, restored)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (!BLeq2(src->size, restored->size))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        const BL1r32_t* i_restored = (const BL1r32_t*)BLarray1D_begin(restored);
        for (uint16_t i = 0; i < src->size[1]; i++)
        {
            if (i_src[i] != i_restored[i])
            {
                err = EXIT_FAILURE;
                break;
            }
        }
        if (err)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
    } while (0);
    BLSAFEFREE(&byte_buffer);
    BLSAFEFREE(&restored);
    BLSAFEFREE(&src);
    UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
    return err;
}