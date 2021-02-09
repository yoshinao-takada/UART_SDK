#ifndef BLBASE_H_
#define BLBASE_H_
#ifdef __cplusplus
#include    <cstdint>
#include    <ctime>
#include    <cstddef>
#include    <cstdio>
#include    <cstdlib>
extern "C" {
#else
#include    <stdint.h>
#include    <time.h>
#include    <stddef.h>
#include    <stdio.h>
#include    <stdlib.h>
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(_a_)      (sizeof(_a_)/sizeof(_a_[0]))
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

#define IF_TRUE_BREAK(_condition_) if (_condition_) { break; }
#define IF_TRUE_SETERRBREAK(_condition_,_err_,_err_value_) if(_condition_) { _err_ = _err_value_; break; }

// Unit test support macros
#define UT_SHOW(_pf_, _testname_, _testline_, _result_) \
    fprintf(_pf_, "%s,%d,%s\n", _testname_, _testline_, !(_result_) ? "SUCCESS" : "FAILURE")
#define UT_SHOWBREAK(_pf_, _testname_, _testline_, _result_) \
    UT_SHOW(_pf_, _testname_, _testline_, _result_); break;

#ifdef __cplusplus
}
#endif
#endif /* BLBASE_H_ */