#ifndef BLSETTINGS_H_
#define BLSETTINGS_H_
#ifdef __cplusplus
#include    <cstdio>
extern "C" {
#else
#include    <stdio.h>
#endif
typedef struct {
    int whole_text_size;
    char *whole_text;
    int item_count;
    const char* *items;
    const char* *item_values;
    int* key_lengths;
} BLsettings_t, *pBLsettings_t;
typedef const BLsettings_t *pcBLsettings_t;

/*!
\brief create a new object reading a file.
\param settings [out] pointer pointer to the new object
\param filepath [in]
*/
int BLsettings_read(pBLsettings_t settings, const char* filepath);

/*!
\brief extract a value selected by key
\param settings [in] settings parser object
\param key [in]
\param value [out]
\return EXIT_SUCCESS: success, others: unix errno compatible code
*/
int BLsettings_getint(pcBLsettings_t settings, const char* key, int* value);
int BLsettings_getint2D(pcBLsettings_t settings, const char* key, int* value);
int BLsettings_getint3D(pcBLsettings_t settings, const char* key, int* value);
int BLsettings_getfloat(pcBLsettings_t settings, const char* key, float* value);
int BLsettings_getfloat2D(pcBLsettings_t settings, const char* key, float* value);
int BLsettings_getfloat3D(pcBLsettings_t settings, const char* key, float* value);

/*!
\brief extract a value string reference selected by key
\param settings [in] settings parser object
\param key [in]
\return NULL if no item_values[index of key] exists else return item_values[index of key].
*/
const char* Blsettings_getstring(pcBLsettings_t settings, const char* key);

/*!
\brief delete an existing object
\param settings [in,out] pointer pointer to the object to delete
*/
void BLsettings_clean(pBLsettings_t settings);

/*!
\brief write all contents in settings for debugging
\param settings [in] object to write
\param pf [in] output destination text file
*/
void BLsettings_write(pcBLsettings_t settings, FILE* pf);
#ifdef __cplusplus
}
#endif
#endif /* BLSETTINGS_H_ */