#include    "UASDK_serialize.h"
#include    <errno.h>

int ID8cc_codes_new(uint32_t element_count, pID8cc_codes_t* pp)
{
    int err = EXIT_SUCCESS;
    do {
        size_t cb_data = element_count * sizeof(ID8cc_code_t);
        size_t cb_head = sizeof(ID8cc_codes_t);
        size_t cb_alloc = cb_data + cb_head;
        pID8cc_codes_t p = (*pp = (pID8cc_codes_t)malloc(cb_alloc));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->element_count = element_count;
    } while (0);
    return err;
}

int ID8cc_codes_newa(uint32_t element_count, pID8cc_codes_t* pp)
{
    int err = EXIT_SUCCESS;
    do {
        size_t cb_data = element_count * sizeof(ID8cc_code_t);
        size_t cb_head = sizeof(ID8cc_codes_t);
        size_t cb_alloc = cb_data + cb_head;
        pID8cc_codes_t p = (*pp = (pID8cc_codes_t)alloca(cb_alloc));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->element_count = element_count;
    } while (0);
    return err;
}

pcID8cc_code_t ID8cc_codes_find(pcID8cc_codes_t codes, const uint8_t* code_cc8)
{
    pcID8cc_code_t p = NULL;
    for (uint32_t i = 0; i < codes->element_count; i++)
    {
        if (cc8_equal(codes->element[i].code, code_cc8))
        {
            p = &codes->element[i];
            break;
        }
    }
    return p;
}

int cc8_equal(const uint8_t code0[8], const uint8_t code1[8])
{
    int are_all_equal = 1;
    for (uint32_t i = 0; i < sizeof(code0); i++)
    {
        if (code0[i] != code1[i])
        {
            are_all_equal = 0;
            break;
        }
    }
    return are_all_equal;
}