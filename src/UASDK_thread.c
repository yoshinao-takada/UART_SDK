#include    "UASDK_thread.h"
#include    <stdlib.h>
#include    <errno.h>
#include    <stdio.h>

static void* UASDK_thread_runner(void* arg);
static void* UASDK_thread_proc_donothing(void* arg) {}

int UASDK_thread_new(pUASDK_thread_t* ppthread, UASDK_callback_t core_procedure, void* core_procedure_arg)
{
    int err = EXIT_SUCCESS;
    do {
        pUASDK_thread_t thread = (*ppthread = (pUASDK_thread_t)malloc(sizeof(UASDK_thread_t)));
        if (!thread)
        {
            err = ENOMEM;
            break;
        }
        thread->core.arg = core_procedure_arg;
        thread->core.procedure = core_procedure;
        if (EXIT_SUCCESS != (err = pthread_mutex_init(&thread->core.sync_gate_enter, NULL)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = pthread_mutex_lock(&thread->core.sync_gate_enter)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = pthread_mutex_init(&thread->core.sync_gate_exit, NULL)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = pthread_mutex_lock(&thread->core.sync_gate_exit)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = pthread_create(&thread->thread, NULL, UASDK_thread_runner, &thread->core)))
        {
            break;
        }
    } while (0);
    if (err && *ppthread)
    {
        free(*ppthread);
        *ppthread = NULL;
    }
    return err;
}

int UASDK_thread_delete(pUASDK_thread_t* ppthread)
{
    int err = EXIT_SUCCESS;
    pUASDK_thread_t thread = *ppthread;
    do {
        thread->core.procedure = UASDK_thread_proc_donothing;
        if (EXIT_SUCCESS != (err = pthread_mutex_unlock(&thread->core.sync_gate_enter)))
        {
            break;
        }
        pUASDK_thread_core_procedure_t thread_core_ptr;
        if (EXIT_SUCCESS != (err = pthread_join(thread->thread, (void**)&thread_core_ptr)))
        {
            break;
        }

        // destroy mutexes
        pthread_mutex_destroy(&thread_core_ptr->sync_gate_enter);
        pthread_mutex_destroy(&thread_core_ptr->sync_gate_exit);

        // destroy **ppthread
        free(*ppthread);
        *ppthread = NULL;
    } while (0);
    return err;
}

static void* UASDK_thread_runner(void* arg)
{
    pUASDK_thread_core_procedure_t core = (pUASDK_thread_core_procedure_t)arg;
    int err = EXIT_SUCCESS;
    while (core->procedure != UASDK_thread_proc_donothing)
    {
        err = pthread_mutex_lock(&core->sync_gate_enter);
        if (core->procedure)
        {
            core->procedure(core->arg);
        }
        err = pthread_mutex_unlock(&core->sync_gate_exit);
    }
    return arg;
}

void UASDK_thread_enter_core_proc(pUASDK_thread_t thread)
{
    pthread_mutex_unlock(&thread->core.sync_gate_enter);
}

void UASDK_thread_wait_core_proc_exit(pUASDK_thread_t thread)
{
    pthread_mutex_lock(&thread->core.sync_gate_exit);
}
