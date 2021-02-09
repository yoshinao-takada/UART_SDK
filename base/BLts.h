#ifndef BLTS_H_
#define BLTS_H_
/*!
\file BLts.h
\brief It is a middleware which makes high precision time measurement and logging easy.
It depends only on time.h and stdio.h. It supplies the following functionalities.
1) elapsed time measurement
2) conversion from timespec to float/double
3) very simple logging writing to a text file
*/
#ifdef __cplusplus
#include    <ctime>
#include    <cstdio>
extern "C" {
#else
#include    <time.h>
#include    <stdio.h>
#endif
/*!
\brief get the current time.
\param t [out] UTC time
*/
void BLts_getnow(struct timespec* t);

/*!
\brief subtract tb from ta
\param ta_minus_tb [out]
\param ta [in] reference time point
\param tb [in] subject time point
*/
void BLts_ta_minus_tb(struct timespec* ta_minus_tb, const struct timespec* ta, const struct timespec* tb);

/*!
\brief elapsed time
\param telapsed [out]
\param tref [in]
*/
void BLts_getelapsed(struct timespec* telapsed, const struct timespec* tref);

/*!
\brief convert to float in sec
\param t [in]
\return converted value in seconds
*/
float BLts_tofs(const struct timespec* t);

/*!
\brief convert to float in millisec
\param t [in]
\return converted value in milliseconds
*/
float BLts_tofms(const struct timespec* t);

/*!
\brief simple logging tool recording elapsed time
\param pf_ [in] output text file stream
\param tref_ [in] pointer to timespec of reference time of elapsed time
\parma ... [in] time is printed followed by the message defined by the variadic variables which
contains printf style format descriptor and variables; e.g. "%s:%d", "hello", 100
*/
#define BLts_logelapsed(pf_, tref_, ...) { \
    struct timespec telapsed_; BLts_getelapsed(&telapsed_, tref_); \
    float tflt_sec = BLts_tofs(&telapsed_); \
    fprintf(pf_, "Te-sec,%f,",tflt_sec_); fprintf(pf_, __VA_ARGS__); \
}

#define BLts_logelapsedms(pf_, tref_, ...) { \
    struct timespec telapsed_; BLts_getelapsed(&telapsed_, tref_); \
    float tflt_millisec_ = BLts_tofms(&telapsed_); \
    fprintf(pf_, "Te-millisec,%f,",tflt_millisec_); fprintf(pf_, __VA_ARGS__); \
}

/*!
\brief simple logging tool recording timestamp represented in seconds of UTC.
*/
#define BLts_log(pf_, ...) { \
    struct timespec tnow_; BLts_getnow(&tnow_); \
    float tfls_sec_ = BLts_tofs(&tnow_); \
    fprintf(pf_, "TUTC-sec,%f,",tfls_sec_); fprintf(pf_, __VA_ARGS__); \
}

#ifdef __cplusplus
}
#endif
#endif /* BLTS_H_ */