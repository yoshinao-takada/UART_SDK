#ifndef BLHEAPDEBUG_H_
#define BLHEAPDEBUG_H_
#include    "base/BLbase.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    uintptr_t address; // used as a search key
    size_t allocated_bytes;
    uintptr_t caller; // caller address
    struct timespec timestamp;
} BLheapentry_t, *pBLheapentry_t;
typedef const BLheapentry_t *pcBLheapentry_t;
#define BLHEAPENTRY_CSV_HEADER  "ADDRESS,ALLOCATED_BYTES,CALLER,TIME\n"
#define ABORTREPORT_CSV     "abort-report.csv" /* abort report is written in current directory */

typedef void* (*hook_malloc_t)(size_t size, const void* caller);
typedef void (*hook_free_t)(void* ptr, const void* caller);
typedef void* (*hook_realloc_t)(void* ptr, size_t size, const void* caller);

typedef struct {
    hook_malloc_t hook_malloc;
    hook_free_t hook_free;
    hook_realloc_t hook_realloc;
} BLheapAPIs_t, *pBLheapAPIs_t;
typedef const BLheapAPIs_t *pcBLheapAPIs_t;

typedef struct {
    BLheapAPIs_t apis;
    int entries_filled;
    int entries_max;
    BLheapentry_t entries[0];
} BLheapdebug_t, *pBLheapdebug_t;
typedef const BLheapdebug_t *pcBLheapdebug_t;

/*!
\brief return the static instance accessible from mymalloc, myfree, and myrealloc.
*/
pBLheapdebug_t BLheapdebug_getwork();

/*!
\brief set the static instance accessible from mymalloc, myfree, and myrealloc.
*/
void BLheapdebug_setwork(pBLheapdebug_t heapdebug);

/*!
\brief create a new object
At the initial state, the object pointer is set to NULL.
\param entries_size [in] array size of the new object
\param ppobj [out] pointer pointer to the new object
\return unix errno compatible error code
*/
int BLheapdebug_new(int entries_size, pBLheapdebug_t* ppobj);

/*!
\brief switch state of disabled/enabled recording heap operation.
At the initial state, it is disabled.
*/
void BLheapdebug_swap_hooks(pBLheapAPIs_t saved);

/*!
\brief save heapdebug contents as a csv file.
\param heapdebug [in] heap entry data
\param filepath [in] csv file path to save to.
\param write_mode [in] "w" or "a", open() write mode flag. Only "w" or "a" is allowed.
\return unix errno compatible error code.
*/
int BLheapdebug_savecsv(pcBLheapdebug_t heapdebug, const char* filepath, const char* write_mode);

/*!
\brief save abort report as a csv file.
\param heapdebug [in] heap entry data
\param filepath [in] csv file path to save to.
\param entry [in] a controvertial heap address info created at the program abortion.
*/
int BLheapdebug_save_abortreport(pcBLheapdebug_t heapdebug, const char* filepath, pcBLheapentry_t entry);
#ifdef __cplusplus
}
#endif
#endif /* BLHEAPDEBUG_H_ */