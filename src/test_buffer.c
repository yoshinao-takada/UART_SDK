#include    "UASDK_buffer.h"
#include    "base/BLbase.h"
#include    "base/BLarray1D.h"
#include    <memory.h>

int UT_UASDK_buffer_shiftout();
int UT_UASDK_buffer_copy();
int UT_BL_array1D_alloc_set_get_free();
int UT_test_rand();

int test_buffer()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = UT_UASDK_buffer_shiftout())) { break; }
        if (EXIT_SUCCESS != (err = UT_UASDK_buffer_copy())) { break; }
    } while (0);
    return err;
}


int UT_UASDK_buffer_shiftout()
{
    static const uint8_t data0[] = { 0,1,2,3,4,5,6,7,8,9,10 };
    uint8_t raw_buffer[16] = { 0 };
    int err = EXIT_SUCCESS;
    pUASDK_buffer_t buffer = NULL;
    do {
        // Step 1: initialize buffer.
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &buffer)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        memcpy((void*)UASDK_buffer_vacant_begin(buffer), (const void*)data0, sizeof(data0));
        buffer->caplen.byte_filled += sizeof(data0);
        if (sizeof(data0) != buffer->caplen.byte_filled)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
        // Step 2: shift out 6 bytes which are less than the current content size in buffer.
        const uint8_t shift_size0 = 6;
        uint8_t byte_count = UASDK_buffer_shiftout((void*)raw_buffer, buffer, shift_size0);
        // Step 2.1: check the shifted out data
        if (byte_count != shift_size0)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        for (uint8_t i = 0; i < byte_count; i++)
        {
            if (raw_buffer[i] != data0[i])
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
            }
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
        // Step 2.2: check the remaining data in buffer
        if (buffer->caplen.byte_filled != (sizeof(data0) - shift_size0))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        for (uint8_t i = 0; i < buffer->caplen.byte_filled; i++)
        {
            if (data0[i + shift_size0] != buffer->head.pbytes[i])
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
            }
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
        // Step 4: try to shift out 16 bytes which are more than the current content size in buffer
        const uint8_t shift_size1 = sizeof(raw_buffer);
        byte_count = UASDK_buffer_shiftout((void*)raw_buffer, buffer, shift_size1);
        if (byte_count != (sizeof(data0) - shift_size0))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        for (uint8_t i = 0; i < byte_count; i++)
        {
            if (raw_buffer[i] != data0[i + shift_size0])
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
            }
        }
        if (buffer->caplen.byte_filled != 0)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
    } while (0);
    BLSAFEFREE(&buffer);
    return err;
}

int UT_UASDK_buffer_copy_fullsuccess();
int UT_UASDK_buffer_copy_lessbysrc();
int UT_UASDK_buffer_copy_lessbydst();
int UT_UASDK_buffer_copy_err();
int UT_UASDK_buffer_copy()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = UT_UASDK_buffer_copy_fullsuccess()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
        if (EXIT_SUCCESS != (err = UT_UASDK_buffer_copy_lessbysrc()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
        if (EXIT_SUCCESS != (err = UT_UASDK_buffer_copy_lessbydst()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
        if (EXIT_SUCCESS != (err = UT_UASDK_buffer_copy_err()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
    } while (0);
    return err;
}

int UT_UASDK_buffer_copy_fullsuccess()
{
    static const uint8_t data0[] = 
    {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };
    int err = EXIT_SUCCESS;
    pUASDK_buffer_t src = NULL, dst = NULL;
    do {
        // Step 1: initialize src, dst
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &src)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &dst)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        memcpy(UASDK_buffer_vacant_begin(src), data0, sizeof(data0));
        src->caplen.byte_filled = sizeof(data0);
        // Step 2: copy
        uint8_t byte_count = src->caplen.byte_filled;
        UASDK_bufcpresult_t copy_result = UASDK_buffer_copy(dst, src, &byte_count);
        if ((UASDK_bufcpresult_fullsuccess != copy_result) || 
            (byte_count != sizeof(data0)) ||
            (dst->caplen.byte_filled != sizeof(data0)) ||
            (src->caplen.byte_filled != 0))
        {
            fprintf(stderr, "copy_result = %d\n", (int)copy_result);
            fprintf(stderr, "byte_count = %d\n", byte_count);
            fprintf(stderr, "dst->caplen.byte_filled = %d\n", dst->caplen.byte_filled);
            fprintf(stderr, "src->caplen.byte_filled = %d\n", src->caplen.byte_filled);
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
    } while (0);
    BLSAFEFREE(&src);
    BLSAFEFREE(&dst);
    return err;
}

int UT_UASDK_buffer_copy_lessbysrc()
{
    static const uint8_t data0[] = 
    {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };
    int err = EXIT_SUCCESS;
    pUASDK_buffer_t src = NULL, dst = NULL;
    do {
        // Step 1: initialize src, dst
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &src)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &dst)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        memcpy(UASDK_buffer_vacant_begin(src), data0, sizeof(data0));
        src->caplen.byte_filled = sizeof(data0);
        // Step 2: copy
        const uint8_t copy_size0 = 5;
        uint8_t byte_count = copy_size0;
        UASDK_bufcpresult_t copy_result = UASDK_buffer_copy(dst, src, &byte_count);
        if ((UASDK_bufcpresult_fullsuccess != copy_result) || 
            (byte_count != copy_size0) ||
            (dst->caplen.byte_filled != byte_count) ||
            (src->caplen.byte_filled != (sizeof(data0) - byte_count)))
        {
            fprintf(stderr, "copy_result = %d\n", (int)copy_result);
            fprintf(stderr, "byte_count = %d\n", byte_count);
            fprintf(stderr, "dst->caplen.byte_filled = %d\n", dst->caplen.byte_filled);
            fprintf(stderr, "src->caplen.byte_filled = %d\n", src->caplen.byte_filled);
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        UASDK_buffer_clear(dst);
        byte_count = dst->caplen.byte_capacity;
        copy_result = UASDK_buffer_copy(dst, src, &byte_count);
        if ((UASDK_bufcpresult_lessbysrc != copy_result) ||
            (byte_count != ((uint8_t)sizeof(data0) - copy_size0)) ||
            (dst->caplen.byte_filled != byte_count) ||
            (src->caplen.byte_filled != 0))
        {
            fprintf(stderr, "copy_result = %d\n", (int)copy_result);
            fprintf(stderr, "byte_count = %d\n", byte_count);
            fprintf(stderr, "dst->caplen.byte_filled = %d\n", dst->caplen.byte_filled);
            fprintf(stderr, "src->caplen.byte_filled = %d\n", src->caplen.byte_filled);
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
    } while (0);
    BLSAFEFREE(&src);
    BLSAFEFREE(&dst);
    return err;
}

int UT_UASDK_buffer_copy_lessbydst()
{
    static const uint8_t data0[] = 
    {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };
    int err = EXIT_SUCCESS;
    pUASDK_buffer_t src = NULL, dst = NULL;
    do {
        // Step 1: initialize src, dst
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &src)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &dst)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        memcpy(UASDK_buffer_vacant_begin(src), data0, sizeof(data0));
        src->caplen.byte_filled = sizeof(data0);
        // Step 2: fill the beginning part of dst.
        const uint8_t filling_byte_count = 6;
        dst->caplen.byte_filled += filling_byte_count;
        // Step 3: copy and check
        uint8_t copy_byte_count = sizeof(data0);
        UASDK_bufcpresult_t copy_result = UASDK_buffer_copy(dst, src, &copy_byte_count);
        if ((copy_result != UASDK_bufcpresult_lessbydst) ||
            (copy_byte_count != (sizeof(data0) - filling_byte_count)) ||
            (dst->caplen.byte_filled != dst->caplen.byte_capacity) ||
            (src->caplen.byte_filled != (sizeof(data0) - copy_byte_count)))
        {
            fprintf(stderr, "copy_result = %d\n", copy_result);
            fprintf(stderr, "copy_byte_count = %d\n", copy_byte_count);
            fprintf(stderr, "dst->caplen.byte_filled = %d\n", dst->caplen.byte_filled);
            fprintf(stderr, "src->caplen.byte_filled = %d\n", src->caplen.byte_filled);
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
    } while (0);
    BLSAFEFREE(&src);
    BLSAFEFREE(&dst);
    return err;
}

int UT_UASDK_buffer_copy_err()
{
    static const uint8_t data0[] = 
    {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };
    int err = EXIT_SUCCESS;
    pUASDK_buffer_t src = NULL, dst = NULL;
    do {
        // Step 1: initialize src, dst
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &src)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(16, &dst)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        memcpy(UASDK_buffer_vacant_begin(src), data0, sizeof(data0));
        src->caplen.byte_filled = sizeof(data0);
        // step 2 try to copy with a nullptr
        uint8_t copy_byte_count = 8;
        UASDK_bufcpresult_t copy_result = UASDK_buffer_copy(NULL, src, &copy_byte_count);
        if (copy_result != UASDK_bufcpresult_error)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
    } while (0);
    BLSAFEFREE(&src);
    BLSAFEFREE(&dst);
    return err;
}

int test_array1D()
{
    int err = EXIT_SUCCESS;
    do {
//        if (EXIT_SUCCESS != (err = UT_test_rand())) { break; }
        if (EXIT_SUCCESS != (err = UT_BL_array1D_alloc_set_get_free())) { break; }
    } while (0);
    return err;
}

int UT_BL_array1D_alloc_set_get_free()
{
    int err = EXIT_SUCCESS;
    pBLarray1D_t a[1024] = { NULL };
    do {
        BL2u16_t size = { sizeof(BL2u32_t), 4096 };
        srand(1); // reset RNG
        for (int i = 0; i != ARRAYSIZE(a); i++)
        {
            BLarray1D_alloc(a[i], size);
            BL2u32_t* p = (BL2u32_t*)BLarray1D_begin(a[i]);
            for (uint16_t j = 0; j < a[i]->size[1]; j++)
            {
                p[j][0] = rand();
                p[j][1] = rand();
            }
        }
        srand(1);
        for (int i = 0; i != ARRAYSIZE(a); i++)
        {
            const BL2u32_t* p2 = (const BL2u32_t*)BLarray1D_begin(a[i]);
            for (uint16_t j = 0; j < a[i]->size[1]; j++)
            {
                if (p2[j][0] != rand()) { err = EXIT_FAILURE; break; }
                if (p2[j][1] != rand()) { err = EXIT_FAILURE; break; }
            }
        }
        UT_SHOW(stdout, __FUNCTION__, __LINE__, err);
    } while (0);
    for (int i = 0; i != ARRAYSIZE(a); i++)
    {
        BLSAFEFREE(&a[i]);
    }
    return err;
}

int UT_test_rand()
{
    int a[16], b[16];
    srand(1);
    for (int i = 0; i != ARRAYSIZE(a); i++)
    {
        a[i] = rand();
    }
    srand(1);
    for (int i = 0; i != ARRAYSIZE(b); i++)
    {
        b[i] = rand();
    }
    for (int i = 0; i != ARRAYSIZE(b); i++)
    {
        fprintf(stdout, "a[%d]=%d, b[%d]=%d\n", i, a[i], i, b[i]);
    }
    return 0;
}