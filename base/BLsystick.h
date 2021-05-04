#ifndef BLSYSTICK_H_
#define BLSYSTICK_H_
#include    "base/BLbase.h"
#include    <sys/time.h>
#include    <signal.h>

// time constant represented in struct timespec
#define BL1msTIMESPEC   { 0, 1000000 }
#define BL10msTIMESPEC  { 0, 10000000 }
#define BL100msTIMESPEC { 0, 100000000 }
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    struct sigaction act;
    struct itimerval t;
} BLsystick_acttime_t, *pBLsystick_acttime_t;
typedef const BLsystick_acttime_t *pcBLsystick_acttime_t;

/*!
\brief set obj->t.
\param obj [in,out] target object
\param time [in] time represented in various types
*/
void BLsystick_acttime_setbytimeval(pBLsystick_acttime_t obj, const struct timeval* time);
void BLsystick_acttime_setbytimespec(pBLsystick_acttime_t obj, const struct timespec* time);
void BLsystick_acttime_setbytvsec(pBLsystick_acttime_t obj, int tv_sec);
void BLsystick_acttime_setbyfloat(pBLsystick_acttime_t obj, float time);
void BLsystick_acttime_setbydouble(pBLsystick_acttime_t obj, double time);

typedef struct {
    BLsystick_acttime_t old;
    BLsystick_acttime_t new;
    BLcallback_t systick_handler;
    void* systick_args;
} BLsystick_t, *pBLsystick_t;
typedef const BLsystick_t *pcBLsystick_t;

/*!
\brief wrapper function for sigaction() API.
\param systick [in,out] working data of setting of sigaction
\return unix errno compatible error code
*/
int BLsystick_set(pBLsystick_t systick);

/*!
\brief wrapper function for sigaction() API to restore systick->old.
*/
int BLsystick_restore(pBLsystick_t systick);



#pragma region SAMPLE_HANDLER
typedef struct {
    int count;
    FILE* output;
} BLsystick_samplecontext_t, *pBLsystick_samplecontext_t;
typedef const BLsystick_samplecontext_t *pcBLsystick_samplecontext_t;

/*!
\brief sample callback signal receives an instance of BLsystick_samplecontext_t.
increment count and print its value to output. It does the following operations.
*/
void* BLsystick_samplecallback(void* param);
#pragma endregion SAMPLE_HANDLER
#ifdef __cplusplus
}
#endif
#endif /* BLSYSTICK_H_ */