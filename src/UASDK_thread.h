#ifndef UASDK_THREAD_H_
#define UASDK_THREAD_H_
#include    "UASDK_defs.h"
#include    <pthread.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    UASDK_callback_t procedure;
    void* arg;
    pthread_mutex_t sync_gate_enter, sync_gate_exit;
} UASDK_thread_core_procedure_t, *pUASDK_thread_core_procedure_t;
typedef const UASDK_thread_core_procedure_t *pcUASDK_thread_core_procedure_t;

typedef struct {
    pthread_t thread;
    UASDK_thread_core_procedure_t core;
} UASDK_thread_t, *pUASDK_thread_t;
typedef const UASDK_thread_t *pcUASDK_thread_t;

/*!
\brief create a new thread object and start it.
\param ppthread [out]
\return errno compatible number
*/
int UASDK_thread_new(pUASDK_thread_t* ppthread, UASDK_callback_t core_procedure, void* core_procedure_arg);

/*!
\brief delete an existing object
\param ppthread [in,out]
*/
int UASDK_thread_delete(pUASDK_thread_t* ppthread);

/*!
\brief It allows to execute core.proceudre() callback function.
\param thread [in,out]
*/
void UASDK_thread_enter_core_proc(pUASDK_thread_t thread);

/*!
\brief wait for core procedure exiting.
\param thread [in,out]
*/
void UASDK_thread_wait_core_proc_exit(pUASDK_thread_t thread);
#ifdef __cplusplus
}
#endif
#endif /* UASDK_THREAD_H_ */