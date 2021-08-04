#include "base/BLsettings.h"
#include "base/BLfile.h"
#include "base/BLbase.h"
#include <uuid/uuid.h>
#include <string.h>
#pragma region read_and_its_supporting_functions
static int BLsettings_read0(pBLsettings_t settings, const char* filepath)
{
    int err = EXIT_SUCCESS;
    FILE* pf = NULL;
    uint32_t filesize = 0;
    do {
        if (!BLfile_exists(filepath))
        {
            err = ENOENT;
            break;
        }
        if (EXIT_SUCCESS != (err = BLfile_size(filepath, &filesize)))
        {
            break;
        }
        settings->whole_text_size = (int)filesize;
        settings->whole_text = (char*)malloc(filesize);
        if (!settings->whole_text)
        {
            err = ENOMEM;
            break;
        }
        if (EXIT_SUCCESS != (err = BLfile_open(&pf, filepath, "r")))
        {
            break;
        }
        filesize = fread(settings->whole_text, settings->whole_text_size, 1, pf);
        if (filesize != 1)
        {
            err = errno;
            break;
        }
    } while (0);
    if (pf)
    {
        fclose(pf);
    }
    return err;
}

/*!
\brief break the text by newline characters
*/
static void BLsettings_breaklines(pBLsettings_t settings)
{
    char* ptr = settings->whole_text;
    for (int i = 0; i < settings->whole_text_size; i++)
    {
        if (*ptr == '\n')
        {
            *ptr = '\0';
        }
        ptr++;
    }
}

/*!
\brief remove comments
*/
static void BLsettings_removecomments(pBLsettings_t settings)
{
    int err = EXIT_SUCCESS;
    int state = 0; // 0: out of comment, 1: in comment
    do {
        char* ptr = settings->whole_text;
        for (int i = 0; i < settings->whole_text_size; i++)
        {
            if (*ptr == '#')
            {
                state = 1;
            }
            else if (*ptr == '\0')
            {
                state = 0;
            }
            if (state)
            {
                *ptr = '\0';
            }
            ptr++;
        }
    } while (0);
}

/*!
\brief count items
\param settings [in,out] the object to tokenize
*/
static int BLsettings_countitems(pBLsettings_t settings)
{
    int err = EXIT_SUCCESS;
    int state = 0; // 0: null region, 1: in line text
    do {
        settings->item_count = 0;
        char* ptr = settings->whole_text;
        for (int i = 0; i < settings->whole_text_size; i++)
        {
            if (state == 0 && *ptr)
            {
                state = 1;
                settings->item_count++;
            }
            else if (state == 1 && *ptr == '\0')
            {
                state = 0;
            }
            ptr++;
        }
        size_t alloc_size = (sizeof(char*) * 2 + sizeof(int)) * settings->item_count;
        settings->items = (const char**)malloc(alloc_size);
        if (settings->items == NULL)
        {
            err = ENOMEM;
            break;
        }
        settings->item_values = settings->items + settings->item_count;
        settings->key_lengths = (int*)(settings->item_values + settings->item_count);
    } while (0);
    return err;
}

/*!
\brief 1-line lexical analysis
\param line_head [in]
\param end_of_text [in]
\param item [out]
\param item_value [out]
\param key_length [out]
*/
static void BLsettings_1linelex(
    const char* line_head,
    const char* end_of_text,
    const char* *item,
    const char* *item_value,
    int* key_length
) {
    const char* key_head = line_head;
    const char* ptr = line_head;
    int end_of_line = 0;
    *item = line_head;
    *item_value = NULL;
    *key_length = 0;
    // skip prefixing white-spaces
    for (; ptr != end_of_text;)
    {
        if ((*ptr) > ' ')
        {
            break;
        }
        ptr++;
    }
    key_head = ptr;
    // get the key
    for (; ptr != end_of_text;)
    {
        if ((*ptr) == '=' || (*ptr) == ' ')
        {
            *key_length = (int)(ptr - key_head);
            break;
        }
        if ((*ptr) == '\0')
        {
            end_of_line++;
            break;
        }
        ptr++;
    }
    if (end_of_line) return;
    // skip '=' and white-spaces between the key and the value
    for (; ptr != end_of_text;)
    {
        if ((*ptr) != '=' && (*ptr) > ' ')
        {
            break;
        }
        if (*ptr == '\0')
        {
            end_of_line++;
            break;
        }
        ptr++;
    }
    if (end_of_line) return;
    *item_value = ptr;
}

/*!
\brief tokenize the whole text
\param settings [in,out] the object to tokenize
*/
static void BLsettings_tokenize(pBLsettings_t settings)
{
    int err = EXIT_SUCCESS;
    int state = 0; // 0: null region, 1: in token
    do {
        
        const char* end_of_text = settings->whole_text + settings->whole_text_size;
        const char* *item = settings->items;
        const char* *item_value = settings->item_values;
        int* key_length = settings->key_lengths;
        for (const char* ptr = settings->whole_text; ptr != end_of_text;)
        {
            if (*ptr == '\0')
            {
                ptr++;
                continue;
            }
            BLsettings_1linelex(ptr, end_of_text, item, item_value, key_length);
            item++;
            item_value++;
            key_length++;
            ptr += strlen(ptr);
        }
    } while (0);
}

int BLsettings_read(pBLsettings_t settings, const char* filepath)
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = BLsettings_read0(settings, filepath)))
        {
            break;
        }
        BLsettings_breaklines(settings);
        BLsettings_removecomments(settings);
        if (EXIT_SUCCESS != (err = BLsettings_countitems(settings)))
        {
            break;
        }
        BLsettings_tokenize(settings);
    } while (0);
    return err;
}
#pragma endregion read_and_its_supporting_functions
#pragma region functions_extracting_value
int BLsettings_getint(pcBLsettings_t settings, const char* key, int* value)
{
    int err = EXIT_SUCCESS;
    const char* value_string = NULL;
    do {
        if (NULL == (value_string = Blsettings_getstring(settings, key)))
        {
            err = ENODATA;
            break;
        }
        *value = atoi(value_string);
    } while (0);
    return err;
}


/*!
\brief parse a comma separated number string like "125, 126", "1.05e-15, 2.08e22"
*/
static int get_next_positions(char* work_buf, int separator_numbers, char* *next_positions)
{
    int err = EXIT_SUCCESS;
    do {
        int separator_count = 0;
        for (char* ptr = work_buf; '\0' != (*ptr); ptr++)
        {
            if (',' == *ptr)
            {
                if (separator_count >= separator_numbers)
                {
                    err = EBADF;
                    break;
                }
                next_positions[separator_count++] = ptr;
            }
        }
        if (err || (separator_numbers != separator_count))
        {
            break;
        }
        for (int i = 0; i < separator_numbers; i++)
        {
            *next_positions[i] = '\0';
            next_positions[i]++;
        }
    } while (0);
    return err;
}

int BLsettings_getint2D(pcBLsettings_t settings, const char* key, int* value)
{
    int err = EXIT_SUCCESS;
    const char* value_string = NULL;
    char* work_buf = NULL;
    do {
        if (NULL == (value_string = Blsettings_getstring(settings, key)))
        {
            err = ENODATA;
            break;
        }
        work_buf = strdup(value_string);
        char* next_values[1];
        if (EXIT_SUCCESS != (err = get_next_positions(work_buf, ARRAYSIZE(next_values), next_values)))
        {
            break;
        }
        value[0] = atoi(work_buf);
        value[1] = atoi(next_values[0]);
    } while (0);
    BLSAFEFREE(&work_buf);
    return err;
}

int BLsettings_getint3D(pcBLsettings_t settings, const char* key, int* value)
{
    int err = EXIT_SUCCESS;
    const char* value_string = NULL;
    char* work_buf = NULL;
    do {
        if (NULL == (value_string = Blsettings_getstring(settings, key)))
        {
            err = ENODATA;
            break;
        }
        work_buf = strdup(value_string);
        char* next_values[2];
        if (EXIT_SUCCESS != (err = get_next_positions(work_buf, ARRAYSIZE(next_values), next_values)))
        {
            break;
        }
        value[0] = atoi(work_buf);
        value[1] = atoi(next_values[0]);
        value[2] = atoi(next_values[1]);
    } while (0);
    BLSAFEFREE(&work_buf);
    return err;
}

int BLsettings_getfloat(pcBLsettings_t settings, const char* key, float* value)
{
    int err = EXIT_SUCCESS;
    const char* value_string = NULL;
    do {
        if (NULL == (value_string = Blsettings_getstring(settings, key)))
        {
            err = ENODATA;
            break;
        }
        *value = atof(value_string);
    } while (0);
    return err;
}

int BLsettings_getfloat2D(pcBLsettings_t settings, const char* key, float* value)
{
    int err = EXIT_SUCCESS;
    const char* value_string = NULL;
    char* work_buf = NULL;
    do {
        if (NULL == (value_string = Blsettings_getstring(settings, key)))
        {
            err = ENODATA;
            break;
        }
        work_buf = strdup(value_string);
        char* next_values[1];
        if (EXIT_SUCCESS != (err = get_next_positions(work_buf, ARRAYSIZE(next_values), next_values)))
        {
            break;
        }
        value[0] = atof(work_buf);
        value[1] = atof(next_values[0]);
    } while (0);
    BLSAFEFREE(&work_buf);
    return err;
}

int BLsettings_getfloat3D(pcBLsettings_t settings, const char* key, float* value)
{
    int err = EXIT_SUCCESS;
    const char* value_string = NULL;
    char* work_buf = NULL;
    do {
        if (NULL == (value_string = Blsettings_getstring(settings, key)))
        {
            err = ENODATA;
            break;
        }
        work_buf = strdup(value_string);
        char* next_values[2];
        if (EXIT_SUCCESS != (err = get_next_positions(work_buf, ARRAYSIZE(next_values), next_values)))
        {
            break;
        }
        value[0] = atof(work_buf);
        value[1] = atof(next_values[0]);
        value[2] = atof(next_values[1]);
    } while (0);
    BLSAFEFREE(&work_buf);
    return err;
}

const char* Blsettings_getstring(pcBLsettings_t settings, const char* key)
{
    int value_string_length = 0;
    int key_length = strlen(key);
    int index = -1;
    for (int i = 0; i < settings->item_count; i++)
    {
        if ((key_length == settings->key_lengths[i]) && (strncmp(key, settings->items[i], key_length) == 0))
        {
            index = i;
            break;
        }
    }
    return (index == -1) ? (const char*)NULL : (const char*)settings->item_values[index];
}
#pragma endregion functions_extracting_value

void BLsettings_clean(pBLsettings_t settings)
{
    settings->whole_text_size = settings->item_count = 0;
    BLSAFEFREE(&settings->whole_text);
    BLSAFEFREE(&settings->items);
}

void BLsettings_write(pcBLsettings_t settings, FILE* pf)
{
    static const size_t item_buf_size = 64;
    char* item_buf = (char*)malloc(item_buf_size);
    fprintf(pf, "whole_text_size = %d\n", settings->whole_text_size);
    fprintf(pf, "item_count = %d\n", settings->item_count);
    for (int i = 0; i < settings->item_count; i++)
    {
        fprintf(pf, "\"%s\": ", settings->items[i]);
        fprintf(pf, "%d: ", settings->key_lengths[i]);
        strncpy(item_buf, settings->items[i], settings->key_lengths[i]);
        item_buf[settings->key_lengths[i]] = '\0';
        fprintf(pf, "\"%s\": ", item_buf);
        if (settings->item_values[i])
        {
            fprintf(pf, "\"%s\"\n", settings->item_values[i]);
        }
        else
        {
            fprintf(pf, "(no value)\n");
        }
    }
    free(item_buf);
}