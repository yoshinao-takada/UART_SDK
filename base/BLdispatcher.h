#ifndef BLDISPATCHER_H_
#define BLDISPATCHER_H_
#include    "base/BLsystick.h"
#include    "base/BLbase.h"
#include    "pthread.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    int32_t down_counter; // down_counter is reloaded at each underflow.
    int32_t down_counter_initial; // initialization value of down_counter
    BLcallback_t handler;
    void* context;
} BLdispatcher_core_t, *pBLdispatcher_core_t;
typedef const BLdispatcher_core_t *pcBLdispatcher_core_t;

typedef struct {
    BLdispatcher_core_t core;
    pthread_mutex_t mutex;
} BLdispatcher_t, *pBLdispatcher_t;
typedef const BLdispatcher_t *pcBLdispatcher_t;

typedef struct {
    size_t item_count; // number of dispatchers
    BLdispatcher_t dispatchers[0];
} BLdispatchers_t, *pBLdispatchers_t;

/*!
\brief create a dispatcher object
\param ppdispatchers [out] pointer-pointer to the dispatcher object
\param item_count [in] number of items in the object
\return unix errno compatible number
*/
int BLdispatchers_new(pBLdispatchers_t* ppdispatchers, size_t item_count);

/*!
\brief release resources of dispatchers
\param ppdispatchers [in,out] pointer-ponter to the dispatcher object
*/
void BLdispatchers_delete(pBLdispatchers_t* ppdispatchers);

/*!
\brief set a dispatcher object parameters
\param dispatchers [in,out] pointer to the dispatcher object
\param index [in] element selector
\param settings [in] setup parameters
\return unix errno compatible number
*/
int BLdispatchers_set(pBLdispatchers_t dispatchers, size_t index, pcBLdispatcher_core_t settings);

/*!
\brief reload the downcounter with its initial value.
\param dispatchers [in,out] pointer to the dispatcher object
\param index [in] element selector
*/
int BLdispatchers_reload(pBLdispatchers_t dispatchers, size_t index);

/*!
\brief clear a dispatcher object
\param dispatchers [in,out] pointer to the dispatcher object
\param index [in] element selector
\return unix errno compatible number
*/
int BLdispatchers_clear(pBLdispatchers_t dispatchers, size_t index);

/*!
\brief dispatch operations
\param dispatchers [in,out] pointer to the dispatcher object
*/
int BLdispatchers_dispatch(pBLdispatchers_t dispatchers);

/*!
\brief initialize global context utilizing a systick timer and a timer dispatcher.
\param dispatcher_count [in] number of dispatcher channels
\param dispatchers [in] initializers of the dispatcher channels
\param systick_interval [in] interval of SIGALRM generation of systick
\return unix errno compatible number
*/
int BLsystickdispatcher_init(
    size_t dispatcher_count,
    pcBLdispatcher_core_t dispatchers,
    const struct timespec* systick_interval
);

/*!
\brief release resources allocated to the global context of a systick timer and a dispatcher.
*/
void BLsystickdispatcher_destroy();

/*!
\brief reload dispatcher downcounter.
\param index [in] dispatcher index
\return unix errno compatible number
*/
int BLsystickdispatcher_reload(size_t index);
#ifdef __cplusplus
}
#endif
#endif /* BLDISPATCHER_H_ */