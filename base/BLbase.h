#ifndef BLBASE_H_
#define BLBASE_H_
#ifdef __cplusplus
#include    <cstdint>
#include    <ctime>
#include    <cstddef>
#include    <cstdio>
#include    <cstdlib>
#include    <cerrno>
#include    <cassert>
extern "C" {
#else
#include    <stdint.h>
#include    <time.h>
#include    <stddef.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <assert.h>
#endif
#include    <pthread.h>
typedef struct {
    pthread_mutex_t  enable_worker;
    pthread_mutex_t  enable_main;
} BLsynchronizer_t, *pBLsynchronizer_t;

#ifndef ARRAYSIZE
#define ARRAYSIZE(_a_)      (sizeof(_a_)/sizeof(_a_[0]))
#endif
#ifndef _countof
#define _countof    ARRAYSIZE
#endif
#ifndef __max
#define __max(_a_,_b_)      ((_a_ > _b_) ? _a_ : _b_)
#endif
#ifndef __min
#define __min(_a_,_b_)      ((_a_ < _b_) ? _a_ : _b_)
#endif

#define BLALIGN(a,N)   (((a) & (N-1)) ? (((a) | (N-1)) + 1) : (a))
#define BLALIGN8(a)    BLALIGN(a,8)
#define BLCALLOC(nmemb,unit_type)  (unit_type*)calloc(nmemb,sizeof(unit_type))
#define BLSAFEFREE(ppobj)  if (*ppobj) { free((void*)(*ppobj)); *ppobj = NULL; }

#ifndef _WIN32
typedef void*   HANDLE;
typedef uint32_t DWORD;
typedef uint64_t ULONGLONG;
#endif
#ifdef __unix__
#define CHR_DIRSEP  '/'
#define STR_DIRSEP  "/"
#elif defined(_WIN32)
#define CHR_DIRSEP  '\\'
#define STR_DIRSEP  "\\"
#endif

// this type is identical to the thread start routine of pthread
typedef void* (*BLcallback_t)(void* param);
// more sophisticated callback function
/*!
\param context [in,out] parameter
\param out [out] output variables
\param in [in] input variables
\return unix errno compatible error code
*/
typedef int (*BLcallback2_t)(void* context, void* out, const void* in);

typedef struct {
    BLcallback2_t func;
    void* context;
    void* out;
    void* in;
} BLcallbackctx_t, *pBLcallbackctx_t;
typedef const BLcallbackctx_t *pcBLcallbackctx_t;

// generic parameter for BLcallback_t functions
typedef struct {
    void* context;
    void* variables;
} BLparams_t, *pBLparams_t;
typedef const BLparams_t *pcBLparams_t;

#define IF_TRUE_BREAK(_condition_) if (_condition_) { break; }
#define IF_TRUE_SETERRBREAK(_condition_,_err_,_err_value_) if(_condition_) { _err_ = _err_value_; break; }

// Unit test support macros
#define UT_SHOW(_pf_, _testname_, _testline_, _result_) \
    fprintf(_pf_, "%s,%d,%s\n", _testname_, _testline_, !_result_ ? "SUCCESS" : "FAILURE")
#define UT_SHOWBREAK(_pf_, _testname_, _testline_, _result_) \
    UT_SHOW(_pf_, _testname_, _testline_, _result_); break;

// nearly equal 
#define BL_EQ_F(x0_, x1_, xtol_) ( \
    ((fabsf(x0_) + fabsf(x1_)) > xtol_) ? \
    ((fabsf((x0_) - (x1_))/(fabsf(x0_) + fabsf(x1_))) < xtol_) : \
    ((fabsf((x0_ + xtol_) - (x1_ + xtol_))/xtol_) < xtol_) \
)

#define BL_EQ(x0_, x1_, xtol_) ( \
    ((fabs(x0_) + fabs(x1_)) > xtol_) ? \
    ((fabs((x0_) - (x1_))/(fabs(x0_) + fabs(x1_))) < xtol_) : \
    ((fabs((x0_ + xtol_) - (x1_ + xtol_))/xtol_) < xtol_) \
)

#define BL_EQ_CF(x0_, x1_, xtol_) (\
    BL_EQ_F(crealf(x0_), crealf(x1_), xtol_) && BL_EQ_F(cimagf(x0_), cimagf(x1_), xtol_) \
)

#define BL_EQ_C(x0_, x1_, xtol_) (\
    BL_EQ(creal(x0_), creal(x1_), xtol_) && BL_EQ(cimag(x0_), cimag(x1_), xtol_) \
)

#define ROTATING_ICONS_CW   "-\\|/-\\|/"
#define ROTATING_ICONS_CCW  "-/|\\-/|\\"
#ifdef __cplusplus
}
#endif
#endif /* BLBASE_H_ */