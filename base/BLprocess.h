#ifndef BLPROCESS_H_
#define BLPROCESS_H_
#include    "base/BLbase.h"
#ifdef __cplusplus
extern "C" {
#endif
/*!
\brief execute a process and wait for its termination.
\param argv [in] command line args
\param exit_code [out] exit code of the process
\return errno compatible function exit status
*/
int BLprocess_execv_sync(char* const argv[], int* exit_code);

/*!
\brief execute a process and return immediately.
\param argv [in] command line args
\param ph_proc [out] process handle
\return errno compatible function exit status
*/
int BLprocess_execv(char* const argv[], HANDLE* ph_proc);

/*!
\brief wait for the process termination
\param h_proc [in] process handle
\param exit_code [out] exit code of the process
\return errno compatible function exit status
*/
int BLprocess_wait_process(HANDLE h_proc, int* exit_code);
#ifdef __cplusplus
}
#endif
#endif /* BLPROCESS_H_ */