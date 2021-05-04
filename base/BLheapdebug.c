#define BLHEAPDEBUG_C_
#include "base/BLheapdebug.h"
#include "base/BLbsearch.h"
#include "base/BLsv.h"
#include "base/BLfile.h"
#include <malloc.h>
#include <pthread.h>

static pBLheapdebug_t s_workdebug = NULL;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pBLheapdebug_t BLheapdebug_getwork()
{
    return s_workdebug;
}

void BLheapdebug_setwork(pBLheapdebug_t heapdebug)
{
    s_workdebug = heapdebug;
}

void BLheapdebug_swap_hooks(pBLheapAPIs_t saved)
{
    BLheapAPIs_t temp_work = { NULL, NULL, NULL };
    temp_work.hook_free = saved->hook_free;
    temp_work.hook_malloc = saved->hook_malloc;
    temp_work.hook_realloc = saved->hook_realloc;
    saved->hook_free = __free_hook;
    saved->hook_malloc = __malloc_hook;
    saved->hook_realloc = __realloc_hook;
    __free_hook = temp_work.hook_free;
    __malloc_hook = temp_work.hook_malloc;
    __realloc_hook = temp_work.hook_realloc;
}

static void* mymalloc(size_t size, const void* caller);
static void myfree(void* ptr, const void* caller);
static void* myrealloc(void* ptr, size_t size, const void* caller);

int BLheapdebug_new(int entries_size, pBLheapdebug_t* ppobj)
{
    int err = EXIT_SUCCESS;
    do {
        if (!ppobj || *ppobj)
        {
            err = EINVAL;
            break;
        }
        size_t cb_head = sizeof(BLheapdebug_t);
        size_t cb_data = sizeof(BLheapentry_t) * entries_size;
        size_t cb_alloc = cb_head + cb_data;
        pBLheapdebug_t p = (*ppobj = (pBLheapdebug_t)malloc(cb_alloc));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        memset(p, 0, cb_alloc);
        p->apis.hook_free = myfree;
        p->apis.hook_malloc = mymalloc;
        p->apis.hook_realloc = myrealloc;
        p->entries_max = entries_size;
    } while (0);
    return err;
}

void BLheapentry_format(pcBLheapentry_t entry, char* buf, size_t bufsize)
{
    snprintf(buf, bufsize, "%jx,%lu,%jx,%d.%8ld\n",
        entry->address, entry->allocated_bytes, entry->caller,
        entry->timestamp.tv_sec, entry->timestamp.tv_nsec);
}

int BLheapdebug_savecsv(pcBLheapdebug_t heapdebug, const char* filepath, const char* write_mode)
{
    char linebuf[128];
    int err = EXIT_SUCCESS;
    FILE* pf = NULL;
    do {
        if (EXIT_SUCCESS != (err = BLfile_open(&pf, filepath, write_mode)))
        {
            fprintf(stderr, "%s,%d,fail in BLfile_open(,%s,\"w\").\n",
                __FILE__, __LINE__, filepath);
            break;
        }
        fputs(BLHEAPENTRY_CSV_HEADER, pf);
        for (int i = 0; i < heapdebug->entries_filled; i++)
        {
            BLheapentry_format(&heapdebug->entries[i], linebuf, ARRAYSIZE(linebuf));
            fputs(linebuf, pf);
        }
    } while (0);
    if (pf)
    {
        fclose(pf);
    }
    return err;
}

int BLheapdebug_save_abortreport(pcBLheapdebug_t heapdebug, const char* filepath, pcBLheapentry_t entry)
{
    int err = EXIT_SUCCESS;
    FILE* pf = NULL;
    do {
        if (EXIT_SUCCESS != (err = BLfile_open(&pf, filepath, "w")))
        {
            fprintf(stderr, "%s,%d,fail in BLfile_open(,%s,\"w\").\n",
                __FILE__, __LINE__, filepath);
            break;
        }
        fprintf(pf, "entry->address = 0x%jx\n", entry->address);
        fprintf(pf, "entry->size = %lu\n", entry->allocated_bytes);
        fprintf(pf, "entry->caller = 0x%jx\n", entry->caller);
        fprintf(pf, "entry->timestamp = %d.%08d\n", entry->timestamp.tv_sec, entry->timestamp.tv_nsec);
        fclose(pf);
        err = BLheapdebug_savecsv(heapdebug, filepath, "a");
    } while (0);
    if (err && pf)
    {
        fclose(pf);
    }
    return err;
}

static int compare_entries(const void* pv0, const void* pv1, const void* params)
{
    int result = 0;
    pcBLheapentry_t pent0 = (pcBLheapentry_t)pv0;
    pcBLheapentry_t pent1 = (pcBLheapentry_t)pv1;
    if (pent0->address > pent1->address)
    {
        result = 1;
    }
    else if (pent0->address < pent1->address)
    {
        result = -1;
    }
    return result;
}
static void* mymalloc(size_t size, const void* caller)
{
    pthread_mutex_lock(&mutex);
    void* return_ptr = NULL;
    // Step 1: disable recording heap operation
    BLheapdebug_swap_hooks(&s_workdebug->apis);

    // Step 2: execute original malloc()
    if (NULL == (return_ptr = malloc(size)))
    {
        abort();
    }
    
    // Step 3: record heap operation
    BLheapentry_t entry = { (uintptr_t)return_ptr, size, (uintptr_t)caller, {0,0}};
    timespec_get(&entry.timestamp, TIME_UTC);
    BL2u32_t range;
    BLbsearch_find((const void*)s_workdebug->entries, s_workdebug->entries_filled, sizeof(BLheapentry_t),
        compare_entries, (const void*)&entry, NULL, range);
    BLbsearch_insert((void*)s_workdebug->entries,  s_workdebug->entries_filled++, sizeof(BLheapentry_t),
        (const void*)&entry, range[1]);

    // Step 4: enable recording heap operation and return
    BLheapdebug_swap_hooks(&s_workdebug->apis);
    pthread_mutex_unlock(&mutex);
    return return_ptr;
}

/*!
\brief return 1 if entry < s_workdebug->entries[0] || entry > s_workdebug->entries[last],
otherwise return 0.
*/
static int outofrange(pcBLheapentry_t entry)
{
    int result = 
    ((entry->address < s_workdebug->entries[0].address) || // less than the minimum
        (entry->address > s_workdebug->entries[s_workdebug->entries_filled-1].address)) // larger than the maximum
    ? 1 : 0;
    return result;
}

/*!
\brief return 1 if s_heapdebug->entries[index] is not match to entry.
*/
static int notmatch(uint32_t index, pcBLheapentry_t entry)
{
    return (s_workdebug->entries[index].address != entry->address) ? 1 : 0;
}
static void myfree(void* ptr, const void* caller)
{
    pthread_mutex_lock(&mutex);
    // Step 1: disable recording heap operation
    BLheapdebug_swap_hooks(&s_workdebug->apis);

    // Step 2: record heap operation
    BLheapentry_t entry = { (uintptr_t)ptr, 0, (uintptr_t)caller, {0,0}};
    timespec_get(&entry.timestamp, TIME_UTC);
    BL2u32_t range;
    BLbsearch_find((const void*)s_workdebug->entries, s_workdebug->entries_filled, sizeof(BLheapentry_t),
        compare_entries, (const void*)&entry, NULL, range);
    if (outofrange(&entry))
    {
        fprintf(stderr, "%s,%d,outofrange\n", __FILE__, __LINE__);
        BLheapdebug_save_abortreport(s_workdebug, ABORTREPORT_CSV, &entry);
        BLSAFEFREE(&s_workdebug);
        abort();
    }
    else if (notmatch(range[0], &entry))
    {
        fprintf(stderr, "%s,%d,nomatch\n", __FILE__, __LINE__);
        BLheapdebug_save_abortreport(s_workdebug, ABORTREPORT_CSV, &entry);
        BLSAFEFREE(&s_workdebug);
        abort();
    }
    BLbsearch_remove((void*)s_workdebug->entries, s_workdebug->entries_filled--, sizeof(BLheapentry_t),
        &entry, range[0]);

    // Step 3: execute original free()
    free(ptr);

    // Step 4: enable recording heap operation
    BLheapdebug_swap_hooks(&s_workdebug->apis);
    pthread_mutex_unlock(&mutex);
}

static void* myrealloc(void* ptr, size_t size, const void* caller)
{
    pthread_mutex_lock(&mutex);
    // Step 1: disable recording heap operation
    BLheapdebug_swap_hooks(&s_workdebug->apis);

    // Step 2: get a new ptr as new_ptr
    void* new_ptr = realloc(ptr, size);
    if (NULL == new_ptr)
    { // error
        fprintf(stderr, "%s,%d,realloc() returned NULL, errno=%d\n", __FILE__, __LINE__, errno);
        BLheapentry_t entry = { (uintptr_t)new_ptr, 0, (uintptr_t)caller, {0, 0} };
        timespec_get(&entry.timestamp, TIME_UTC);
        BLheapdebug_save_abortreport(s_workdebug, ABORTREPORT_CSV, &entry);
        abort();
    }
    BLheapentry_t new_entry = { (uintptr_t)new_ptr, size, (uintptr_t)caller, { 0, 0 } };
    timespec_get(&new_entry.timestamp, TIME_UTC);
    BL2u32_t range;
    if (ptr && (ptr != new_ptr))
    { // replace the entry
        BLheapentry_t old_entry = { (uintptr_t)ptr, 0, 0, { 0, 0 } };
        BLbsearch_find((const void*)s_workdebug->entries, s_workdebug->entries_filled, sizeof(BLheapentry_t),
            compare_entries, (const void*)&old_entry, NULL, range);
        if (outofrange(&old_entry))
        {
            fprintf(stderr, "%s,%d,outofrange\n", __FILE__, __LINE__);
            BLheapdebug_save_abortreport(s_workdebug, ABORTREPORT_CSV, &old_entry);
            BLSAFEFREE(&s_workdebug);
            abort();
        }
        else if (notmatch(range[0], &old_entry))
        {
            fprintf(stderr, "%s,%d,nomatch\n", __FILE__, __LINE__);
            BLheapdebug_save_abortreport(s_workdebug, ABORTREPORT_CSV, &old_entry);
            BLSAFEFREE(&s_workdebug);
            abort();
        }
        BLbsearch_remove((void*)s_workdebug->entries, s_workdebug->entries_filled--, sizeof(BLheapentry_t),
            &old_entry, range[0]);
    }
    BLbsearch_find((const void*)s_workdebug->entries, s_workdebug->entries_filled, sizeof(BLheapentry_t),
        compare_entries, (const void*)&new_entry, NULL, range);
    BLbsearch_insert((void*)s_workdebug->entries,  s_workdebug->entries_filled++, sizeof(BLheapentry_t),
        (const void*)&new_entry, range[1]);

    // Step 4: enable recording heap operation
    BLheapdebug_swap_hooks(&s_workdebug->apis);
    pthread_mutex_unlock(&mutex);
    return new_ptr;
}