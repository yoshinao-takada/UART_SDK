#include    "serialize/UASDK_string.h"
#include    "base/BLbase.h"
#include    <string.h>

int UT_string_new();
int UT_string_serialize_deserialize();

int test_UASDK_string()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = UT_string_new())) { break; }
        if (EXIT_SUCCESS != (err = UT_string_serialize_deserialize())) { break; }
    } while (0);
    return err;
}

int UT_string_new()
{
    static const char* test_src = "0123456";
    static const char test_src_bin[] = { '0', '1', '2', '\0', '3', '4', '5', '\0' };
    int err = EXIT_SUCCESS;
    pUASDK_string_t str = NULL;
    do {
        // Step 1: test UASDK_string_new()
        #pragma region step1
        if (EXIT_SUCCESS != (err = UASDK_string_new(test_src, &str)))
        {
            break;
        }
        if ((1 != str->size[0]) || (8 != str->size[1]))
        {
            fprintf(stderr, "%s,%d,str->size={%d,%d}\n",
                __FUNCTION__, __LINE__, str->size[0], str->size[1]);
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        if (strcmp(test_src, BLarray1D_begin(str)))
        {
            const char* pbuffer = BLarray1D_begin(str);
            fprintf(stderr, "str->buffer={0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x}\n",
                pbuffer[0], pbuffer[1], pbuffer[2], pbuffer[3], pbuffer[4], pbuffer[5], pbuffer[6], pbuffer[7]);
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        BLSAFEFREE(&str);
        #pragma endregion step1
        // Step 2: test UASDK_string_new2()
        #pragma region step2
        str = (pBLarray1D_t)NULL;
        if (EXIT_SUCCESS != (err = UASDK_string_new2(sizeof(test_src_bin), test_src_bin, &str)))
        {
            break;
        }
        if ((1 != str->size[0]) || (8 != str->size[1]))
        {
            fprintf(stderr, "%s,%d,str->size={%d,%d}\n",
                __FUNCTION__, __LINE__, str->size[0], str->size[1]);
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        if (!BLeq8(test_src_bin, BLarray1D_begin(str)))
        {
            const char* pbuffer = BLarray1D_begin(str);
            fprintf(stderr, "str->buffer={0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x}\n",
                pbuffer[0], pbuffer[1], pbuffer[2], pbuffer[3], pbuffer[4], pbuffer[5], pbuffer[6], pbuffer[7]);
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        BLSAFEFREE(&str);
        #pragma endregion step2
    } while(0);
    UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
    return err;
}

int UT_string_serialize_deserialize()
{
    const char* src_string = "01234567";
    int err = EXIT_SUCCESS;
    pUASDK_string_t original = (pUASDK_string_t)NULL, restored = (pUASDK_string_t)NULL;
    uint8_t* byte_buffer = (uint8_t*)NULL;
    do {
        // Step 1: create an object of UASDK_string_t
        if (EXIT_SUCCESS != (err = UASDK_string_new(src_string, &original)))
        {
            break;
        }
        // Step 2: serialize
        int estimate_serialized = UASDK_string_serialize_estimate((const void*)original);
        if (estimate_serialized != (8 + 2 + 9))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        byte_buffer = (uint8_t*)malloc(estimate_serialized);
        if (EXIT_SUCCESS != (err = UASDK_string_serialize((const void*)original, byte_buffer)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        // Step 3: deserialize
        int estimate_deserialized = UASDK_string_deserialize_estimate((uint16_t)estimate_serialized, byte_buffer);
        if (estimate_deserialized != (4 + 9))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        restored = (pUASDK_string_t)calloc(1, estimate_deserialized);
        err = UASDK_string_deserialize((uint16_t)estimate_serialized, byte_buffer, restored);
        if (err)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        // Step 4: check the results
        if (!BLeq2(original->size, restored->size))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        const uint8_t* iorg = (const uint8_t*)BLarray1D_begin(original);
        const uint8_t* ires = (const uint8_t*)BLarray1D_begin(restored);
        for (uint16_t i = 0; i < original->size[1]; i++)
        {
            if (iorg[i] != ires[i])
            {
                err = EXIT_FAILURE;
                break;
            }
        }
        if (err)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
    } while(0);
    BLSAFEFREE(&original);
    BLSAFEFREE(&restored);
    BLSAFEFREE(&byte_buffer);
    UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
    return err;
}
