#include    "base/BLthread.h"
#include    "base/BLbase.h"
#include    <stdlib.h>
#include    <errno.h>

static void* thread_runner(void* param)
{
    pBLthread_t thread = (pBLthread_t)param;
    while (thread->request == BLthread_request_none)
    {
        IF_TRUE_SETERRBREAK(EXIT_SUCCESS != pthread_mutex_lock(&thread->sync_gate_enter), thread->exit_code, errno);
        if (thread->request == BLthread_request_exit_ASAP) break;
        thread->core.procedure(thread->core.arg);
        IF_TRUE_SETERRBREAK(EXIT_SUCCESS != pthread_mutex_unlock(&thread->sync_gate_exit), thread->exit_code, errno);
    }
    return param;
}

int BLthread_new(pBLthread_t *ppthread, BLcallback_t core_proc, void* core_proc_params)
{
    const enum {
        memory_allocated=1,
        sync_gate_enter_initialized=2,
        sync_gate_exit_initialized=4,
        thread_runner_started=8,
    } init_status;
    int err = EXIT_SUCCESS;
    int init_status_flags = 0;
    pBLthread_t thread = (pBLthread_t)NULL;
    do {
        thread = (*ppthread = (pBLthread_t)calloc(1, sizeof(BLthread_t)));
        IF_TRUE_SETERRBREAK(!thread, err, ENOMEM);
        init_status_flags |= memory_allocated;
        thread->core.arg = core_proc_params;
        thread->core.procedure = core_proc;

        IF_TRUE_SETERRBREAK(EXIT_SUCCESS != pthread_mutex_init(
            &thread->sync_gate_enter, (pthread_mutexattr_t*)NULL), err, errno);
        init_status_flags |= sync_gate_enter_initialized;
        IF_TRUE_SETERRBREAK(EXIT_SUCCESS != pthread_mutex_init(
            &thread->sync_gate_exit, (pthread_mutexattr_t*)NULL), err, errno);
        init_status_flags |= sync_gate_exit_initialized;
        IF_TRUE_SETERRBREAK(EXIT_SUCCESS != pthread_mutex_lock(&thread->sync_gate_enter), err, errno);
        IF_TRUE_SETERRBREAK(EXIT_SUCCESS != pthread_mutex_lock(&thread->sync_gate_exit), err, errno);
        IF_TRUE_SETERRBREAK(
            EXIT_SUCCESS != 
                pthread_create(
                    &thread->thread, (const pthread_attr_t*)NULL, thread_runner, (void*)thread),
            err, errno);
        init_status_flags |= thread_runner_started;
    } while (0);
    if (err)
    {
        if (init_status_flags & sync_gate_enter_initialized)
        {
            pthread_mutex_destroy(&thread->sync_gate_enter);
        }
        if (init_status_flags & sync_gate_exit_initialized)
        {
            pthread_mutex_destroy(&thread->sync_gate_exit);
        }
        if (init_status_flags & memory_allocated)
        {
            free((void*)thread);
            *ppthread = (pBLthread_t)NULL;
        }
    }
    return err;
}

/*!
\brief delete an object
\param ppthread [in,out]
*/
int BLthread_delete(pBLthread_t *ppthread)
{
    int err = EXIT_SUCCESS;
    pBLthread_t p = *ppthread;
    do {
        pthread_join(p->thread, NULL);
        err = p->exit_code;
        pthread_mutex_destroy(&(*ppthread)->sync_gate_enter);
        pthread_mutex_destroy(&(*ppthread)->sync_gate_exit);
        free((void*)p);
        *ppthread = (pBLthread_t)NULL;
    } while (0);
    return err;
}

/*!
\brief It allows the core procedure to execute if it is waiting on sync_gate_enter.
\param thread [in,out]
\return errno compatible number
*/
int BLthread_enter_core_proc(pBLthread_t thread, BLthread_request_t request)
{
    int err = EXIT_SUCCESS;
    do {
        thread->request = request;
        IF_TRUE_SETERRBREAK(EXIT_SUCCESS != pthread_mutex_unlock(&thread->sync_gate_enter), err, errno);
    } while (0);
    return err;
}

/*!
\brief wait for core procedure exiting
\param thread [in,out]
\return errno compatible number
*/
int BLthread_wait_core_proc_exit(pBLthread_t thread)
{
    int err = EXIT_SUCCESS;
    do {
        IF_TRUE_SETERRBREAK(EXIT_SUCCESS != pthread_mutex_lock(&thread->sync_gate_exit), err, errno);
    } while (0);
    return err;
}
