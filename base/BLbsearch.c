#include    "base/BLbsearch.h"
#include    <memory.h>

int BLbsearch_inrange(
    const void* array, size_t nmemb, size_t size, BLcompare_t comp,
    const void* target, const void* param, const BL2u32_t range)
{
    const uint8_t* arrayu = (const uint8_t*)array;
    const void* pv0 = (const void*)(arrayu + range[0] * size);
    const void* pv1 = (const void*)(arrayu + range[1] * size);
    int trival0 = comp(pv0, target, param);
    int trival1 = comp(pv1, target, param);
    return ((trival0 <= 0) && (trival1 > 0)) ? 1 : 0;
}

void BLbsearch_find(
    const void* array, size_t nmemb, size_t size, BLcompare_t comp,
    const void* target, const void* param, BL2u32_t range)
{
    const uint8_t *arrayu = (const uint8_t *)array;
    do {
        if (-1 == comp(target, array, param))
        {
            range[0] = range[1] = 0;
            break;
        }
        if (0 <= comp(target, (const void*)(arrayu + (nmemb-1) * size), param))
        {
            range[0] = nmemb - 1;
            range[1] = nmemb;
            break;
        }
        range[0] = 0; range[1] = nmemb - 1;
        do {
            uint32_t mid = (range[0] + range[1]) >> 1;
            if (mid > range[0])
            { // do comparison
                BL2u32_t range0 = { range[0], mid }, range1 = { mid, range[1] };
                if (BLbsearch_inrange(array, nmemb, size, comp, target, param, range0))
                {
                    range[1] = mid;
                }
                else
                {
                    range[0] = mid;
                }
            }
        } while ((range[1] - range[0]) > 1);
    } while (0);
}

int BLbsearch_insert(void* array, size_t nmemb, size_t size,
    const void* new_element, size_t insert_at)
{
    int err = EXIT_SUCCESS;
    void* temp_buffer = NULL;
    do {
        if (insert_at > nmemb)
        {
            err = EINVAL;
            break;
        }
        if (insert_at == nmemb)
        { // append at the last
            void* dst = (void*)(nmemb * size + (uint8_t*)array);
            memcpy(dst, new_element, size);
            break;
        }
        size_t moved_bytes = (nmemb - insert_at) * size;
        temp_buffer = malloc(moved_bytes);
        void* pv_at = (void*)(insert_at * size + (uint8_t*)array);
        memcpy(temp_buffer, pv_at, moved_bytes);
        memcpy(pv_at, new_element, size);
        void* restore_at = (void*)(size + (uint8_t*)pv_at);
        memcpy(restore_at, temp_buffer, moved_bytes);
    } while (0);
    BLSAFEFREE(&temp_buffer);
    return err;
}

int BLbsearch_remove(void* array, size_t nmemb, size_t size,
    void* moved_element, size_t to_remove)
{
    int err = EXIT_SUCCESS;
    do {
        if (to_remove >= nmemb)
        {
            err = ERANGE;
            break;
        }
        void* pv_at = (void*)(to_remove * size + (uint8_t*)array);
        memcpy(moved_element, pv_at, size);
        if (to_remove == (size - 1)) break;
        const void* pv_from = (void*)(size + (uint8_t*)pv_at);
        size_t move_bytes = (nmemb - to_remove - 1) * size;
        memcpy(pv_at, pv_from, move_bytes);
    } while (0);
    return err;
}