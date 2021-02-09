#include    <stdlib.h>
#include    <stdio.h>
#include    <strings.h>
#include    <string.h>
#include    <errno.h>
#include    <math.h>
#ifndef ARRAYSIZE
#define ARRAYSIZE(a__)      sizeof(a__)/sizeof(a__[0])
#endif
#define STIM_CHANNELS   4
#define MEAS_CHANNELS   (STIM_CHANNELS*STIM_CHANNELS)

typedef struct {
    int channel; // channel selector
    float value;
} cmd_params_t, *pcmd_params_t;
typedef const cmd_params_t *pccmd_params_t;

typedef struct {
    float stim_ch_current[STIM_CHANNELS];
    float stim_ch_voltage[STIM_CHANNELS];
    float meas_ch_current[MEAS_CHANNELS];
    float meas_ch_voltage[MEAS_CHANNELS];
} control_object_t, *pcontrol_object_t;
typedef const control_object_t *pccontrol_object_t;

static control_object_t co;

void set_meas_channels()
{
    for (int i = 0; i != STIM_CHANNELS; i++)
    {
        for (int j = 0; j != STIM_CHANNELS; j++)
        {
            int meas_channel = i * STIM_CHANNELS + j;
            co.meas_ch_current[meas_channel] = co.stim_ch_current[i] - co.stim_ch_current[j];
            co.meas_ch_voltage[meas_channel] = co.stim_ch_voltage[i] + co.stim_ch_voltage[j];
        }
    }
}

int init()
{
    int err = EXIT_SUCCESS;
    do {
        for (int i = 0; i != STIM_CHANNELS; i++)
        {
            co.stim_ch_current[i] = co.stim_ch_voltage[i] = 0.0f;
        }
        set_meas_channels();
    } while (0);
    return err;
}

const char* help_string =
"quit : terminate the program\n"
"getm : return channel count of measurement channels\n"
"gets : return channel count of stimulus channels\n"
"getv(N), N = 0..15 : return the voltage on the terminal N\n"
"geti(N), N = 0..15 : return the sink current on the terminal N\n"
"setv(N) <Fv>, N = 0..15 : set the voltage to a floating number Fv\n"
"seti(N) <Fi>, N = 0..15 : set the current to a floating number Fi\n";

typedef struct {
    int i; const char* str;
} int_str_pair_t, *pint_str_pair_t;
typedef const int_str_pair_t *pcint_str_pair_t;

typedef int (*TEST2OP)(const void* in_param, void* out_param);
int test2quit(const void* in_param, void* out_param);
int test2getm(const void* in_param, void* out_param);
int test2gets(const void* in_param, void* out_param);
int test2getv(const void* in_param, void* out_param);
int test2geti(const void* in_param, void* out_param);
int test2setv(const void* in_param, void* out_param);
int test2seti(const void* in_param, void* out_param);

const TEST2OP op[] = 
{
    test2quit, test2getm, test2gets,
    test2getv, test2geti, test2setv, test2seti
};

typedef enum {
    INT_KEY_QUIT,
    INT_KEY_GETM,
    INT_KEY_GETS,
    INT_KEY_GETV,
    INT_KEY_GETI,
    INT_KEY_SETV,
    INT_KEY_SETI,
    INT_KEY_COUNT
} int_keys_t;

static const int_str_pair_t keys[] =
{
    { (int)INT_KEY_QUIT, "quit" },
    { (int)INT_KEY_GETM, "getm" },
    { (int)INT_KEY_GETS, "gets" },
    { (int)INT_KEY_GETV, "getv" },
    { (int)INT_KEY_GETI, "geti" },
    { (int)INT_KEY_SETV, "setv" },
    { (int)INT_KEY_SETI, "seti" }
};

// return EXIT_SUCCESS if c >= ' '
int is_normal_character(char c)
{
    return (c >= ' ') ? EXIT_SUCCESS : EXIT_FAILURE;
}

void rm_newline(char* s)
{
    char* s_scanner = s + strlen(s) - 1;
    while (EXIT_SUCCESS != is_normal_character(*s) && (s_scanner != s))
    {
        *s_scanner-- = '\0';
    }
}

int parse(const char* s)
{
    int item_number = -1;
    do {
        for (int i = 0; i != (int)INT_KEY_COUNT; i++)
        {
            const char* str = keys[i].str;
            if (0 == strncasecmp(s, str, strlen(str)))
            {
                item_number = i;
                break;
            }
        }
    } while (0);
    return item_number;
}

typedef enum {
    NUM_TYPE_NONE,
    NUM_TYPE_INT,
    NUM_TYPE_FLOAT,
} num_type_t;

struct typed_var {
    int type;
    union {
        int i;
        float f;
    } var;
} typed_var;


#pragma region individual_operation
int test2quit(const void* in_param, void* out_param)
{
    struct typed_var* var = (struct typed_var*)out_param;
    var->type = NUM_TYPE_NONE;
    return ENODATA;
}

int test2getm(const void* in_param, void* out_param)
{
    int err = EXIT_SUCCESS;
    struct typed_var* var = (struct typed_var*)out_param;
    do {
        var->type = NUM_TYPE_INT;
        var->var.i = MEAS_CHANNELS;
    } while (0);
    return err;
}

int test2gets(const void* in_param, void* out_param)
{
    int err = EXIT_SUCCESS;
    struct typed_var* var = (struct typed_var*)out_param;
    do {
        var->type = NUM_TYPE_INT;
        var->var.i = STIM_CHANNELS;
    } while (0);
    return err;
}

int parse_index(const char* s, const char* *snext, int* index)
{
    int err = EXIT_SUCCESS;
    do {
        for (; (*s != '(') && (*s != '\0'); s++)
        {
            ; // do nothing
        }
        if (*s == '\0')
        {
            err = EINVAL;
            break;
        }
        else
        {
            *index = 0;
            s++;
            for (; (*s != ')') && (*s != '\0'); s++)
            {
                if (('0' <= *s) && (*s <= '9'))
                {
                    *index = *index * 10 + (int)(*s - '0');
                }
                else
                {
                    err = EINVAL;
                    break;
                }                
            }
            if (*s == '\0')
            {
                err = EINVAL;
            }
            if (err) break;
        }
        *snext = s + 1;
    } while (0);
    return err;
}

int test2getv(const void* in_param, void* out_param)
{
    int err = EXIT_SUCCESS;
    struct typed_var* var = (struct typed_var*)out_param;
    do {
        const char* s = (const char*)in_param;
        const char* snext = NULL;
        int index = -1;
        if (EXIT_SUCCESS != (err = parse_index(s, &snext, &index)))
        {
            break;
        }
        if ((index < 0) || (MEAS_CHANNELS <= index))
        {
            err = ERANGE;
            break;
        }
        var->type = NUM_TYPE_FLOAT;
        var->var.f = co.meas_ch_voltage[index];
    } while (0);
    return err;
}

int test2geti(const void* in_param, void* out_param)
{
    int err = EXIT_SUCCESS;
    struct typed_var* var = (struct typed_var*)out_param;
    do {
        const char* s = (const char*)in_param;
        const char* snext = NULL;
        int index = -1;
        if (EXIT_SUCCESS != (err = parse_index(s, &snext, &index)))
        {
            break;
        }
        if ((index < 0) || (MEAS_CHANNELS <= index))
        {
            err = ERANGE;
            break;
        }
        var->type = NUM_TYPE_FLOAT;
        var->var.f = co.meas_ch_current[index];
    } while (0);
    return err;
}

int test2setv(const void* in_param, void* out_param)
{
    int err = EXIT_SUCCESS;
    struct typed_var* var = (struct typed_var*)out_param;
    do {
        const char* s = (const char*)in_param;
        const char* snext = NULL;
        int index = -1;
        if (EXIT_SUCCESS != (err = parse_index(s, &snext, &index)))
        {
            break;
        }
        if ((index < 0) || (STIM_CHANNELS <= index))
        {
            err = ERANGE;
            break;
        }
        if (*snext < ' ')
        {
            err = EINVAL;
            break;
        }
        co.stim_ch_voltage[index] = atof(snext);
        set_meas_channels();
    } while (0);
    return err;
}

int test2seti(const void* in_param, void* out_param)
{
    struct typed_var* var = (struct typed_var*)out_param;
    int err = EXIT_SUCCESS;
    do {
        const char* s = (const char*)in_param;
        const char* snext = NULL;
        int index = -1;
        if (EXIT_SUCCESS != (err = parse_index(s, &snext, &index)))
        {
            break;
        }
        if ((index < 0) || (STIM_CHANNELS <= index))
        {
            err = ERANGE;
            break;
        }
        if (*snext < ' ')
        {
            err = EINVAL;
            break;
        }
        co.stim_ch_current[index] = atof(snext);
        set_meas_channels();
    } while (0);
    return err;
}

#pragma endregion individual_operation
#pragma region unit_tests
#pragma endregion unit_tests

#ifdef UNIT_TEST
int is_in_tol(float fref, float ftest, float ftol)
{
    return fabsf(fref - ftest) < ftol;
}

// test int parse(const char* s)
int UT_parse()
{
    static const char* sample_strings[] = {
        "quit",
        "getm",
        "gets",
        "getv(10)",
        "geti(7)",
        "setv(3) 1.25",
        "seti(2) -0.34"
    };
    static const int_keys_t keys[] = {
        INT_KEY_QUIT,
        INT_KEY_GETM,
        INT_KEY_GETS,
        INT_KEY_GETV,
        INT_KEY_GETI,
        INT_KEY_SETV,
        INT_KEY_SETI
    };
    int err = EXIT_SUCCESS;

    do {
        for (int i = 0; i != ARRAYSIZE(sample_strings); i++)
        {
            int cmd_index = parse(sample_strings[i]);
            if (cmd_index != (int)keys[i])
            {
                printf("fail in parsing \"%s\"\n", sample_strings[i]);
                err = EFAULT;
                break;
            }
        }

        const char* s_next = NULL;
        int index = -1;
        err = parse_index(sample_strings[2], &s_next, &index);
        if ((err != EINVAL) || (index != -1))
        {
            err = EINVAL;
            break;
        }
        else
        {
            err = EXIT_SUCCESS;
        }
        err = parse_index(sample_strings[3], &s_next, &index);
        if ((err != EXIT_SUCCESS) || (index != 10))
        {
            err = EINVAL;
            break;
        }
        err = parse_index(sample_strings[4], &s_next, &index);
        if ((err != EXIT_SUCCESS) || (index != 7))
        {
            err = EINVAL;
            break;
        }
        err = parse_index(sample_strings[5], &s_next, &index);
        if ((err != EXIT_SUCCESS) || (index != 3))
        {
            err = EINVAL;
            break;
        }
        else
        {
            float f = atof(s_next);
            if (!is_in_tol(1.25f, f, 1.0e-5f))
            {
                err = EINVAL;
                break;
            }
        }
        err = parse_index(sample_strings[6], &s_next, &index);
        if ((err != EXIT_SUCCESS) || (index != 2))
        {
            err = EINVAL;
            break;
        }
        else
        {
            float f = atof(s_next);
            if (!is_in_tol(-0.34f, f, 1.0e-5f))
            {
                err = EINVAL;
                break;
            }
        }
    } while (0);
    return err;
}

int UT_quit()
{
    static const char* sample_string = "QuIt";
    int err = EXIT_SUCCESS;
    do {
        err = test2quit((const void*)sample_string, (void*)&typed_var);
        if (err != ENODATA)
        {
            err = EINVAL;
            break;
        }
        else
        {
            err = EXIT_SUCCESS;
        }
    } while (0);
    return err;
}

int UT_getms()
{
    static const char* sample_strings[] = { "getm", "gets" };
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = test2getm((const void*)sample_strings[0], (void*)&typed_var)))
        {
            break;
        }
        if ((typed_var.type != NUM_TYPE_INT) || (typed_var.var.i != MEAS_CHANNELS))
        {
            err = EFAULT;
            break;
        }
        if (EXIT_SUCCESS != (err = test2gets((const void*)sample_strings[1], (void*)&typed_var)))
        {
            break;
        }
        if ((typed_var.type != NUM_TYPE_INT) || (typed_var.var.i != STIM_CHANNELS))
        {
            err = EFAULT;
            break;
        }
    } while (0);
    return err;
}

int UT_setv()
{
    const char* sample_strings[] = {
        "setv(0) 1.2", "setv(1) -1.2", "setv(2) -0.5", "setv(3) 2"
    };
    int err = EXIT_SUCCESS;
    for (int i = 0; i != ARRAYSIZE(sample_strings);i++)
    {
        if (EXIT_SUCCESS != (err = test2setv((const void*)sample_strings[i], (void*)&typed_var)))
        {
            break;
        }
        for (int i = 0; i != STIM_CHANNELS; i++)
        {
            printf((i == 0) ? "%f" : "\t%f", co.stim_ch_voltage[i]);
        }
        printf("\n");
        for (int i = 0; i != STIM_CHANNELS; i++)
        {
            for (int j = 0; j != STIM_CHANNELS; j++)
            {
                printf((j == 0) ? "%f" : "\t%f", co.meas_ch_voltage[i*STIM_CHANNELS + j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return err;
}

int UT_seti()
{
    const char* sample_strings[] = {
        "seti(0) 0.1", "seti(1) 0.2", "seti(2) -0.2", "seti(3) -0.3"
    };
    int err = EXIT_SUCCESS;
    for (int i=0; i != ARRAYSIZE(sample_strings); i++)
    {
        if (EXIT_SUCCESS != (err = test2seti((const void*)sample_strings[i], (void*)&typed_var)))
        {
            break;
        }
        for (int i = 0; i != STIM_CHANNELS; i++)
        {
            printf((i == 0) ? "%f" : "\t%f", co.stim_ch_current[i]);
        }
        printf("\n");
        for (int i = 0; i != STIM_CHANNELS; i++)
        {
            for (int j = 0; j != STIM_CHANNELS; j++)
            {
                printf((j == 0) ? "%f" : "\t%f", co.meas_ch_current[i*STIM_CHANNELS + j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return err;
}
int main(int argc, const char* argv[])
{
    printf("UNIT_TEST\n");
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS == UT_parse())
        {
            printf("UT_parse() success\n");
        }
        else
        {
            printf("UT_parse() fail\n");
        }
        if (EXIT_SUCCESS == UT_quit())
        {
            printf("UT_quit() success\n");
        }
        else
        {
            printf("UT_quit() fail\n");
        }
        if (EXIT_SUCCESS == UT_getms())
        {
            printf("UT_getms() success\n");
        }
        else
        {
            printf("UT_getms() fail\n");
        }
        if (EXIT_SUCCESS == UT_setv())
        {
            printf("UT_setv() success\n");
        }
        else
        {
            printf("UT_setv() fail\n");
        }
        if (EXIT_SUCCESS == UT_seti())
        {
            printf("UT_seti() success\n");
        }
        else
        {
            printf("UT_seti() fail\n");
        }
    } while (0);
    return err;
}
#else
int main(int argc, const char* argv[])
{
    int err = EXIT_SUCCESS;
    char buf[64];
    do {
        if (EXIT_SUCCESS != (err = init()))
        {
            break;
        }
        for (;;)
        {
            char* s = fgets(buf, ARRAYSIZE(buf), stdin);
            rm_newline(s);
            int item_number = parse(s);
            if (item_number != -1)
            {
                err = op[item_number]((const void*)s, (void*)&typed_var);
                if (ENODATA == err)
                {
                    break;
                }
                if ((item_number == (int)INT_KEY_GETM) || (item_number == (int)INT_KEY_GETS))
                {
                    printf("int number = %d\n", typed_var.var.i);
                }
                else if ((item_number == (int)INT_KEY_GETI) || (item_number == (int)INT_KEY_GETV))
                {
                    printf("float number = %f\n", typed_var.var.f);
                }
            }
            else
            {
                printf("%s", help_string);
            }            
        }
    } while (0);
    return err;
}
#endif