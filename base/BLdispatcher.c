#include    "base/BLdispatcher.h"
#include    <stdbool.h>

int BLdispatchers_new(pBLdispatchers_t* ppdispatchers, size_t item_count)
{
    int err = EXIT_SUCCESS;
    do {
        size_t cb_header = sizeof(BLdispatchers_t);
        size_t cb_elements = item_count * sizeof(BLdispatcher_t);
        size_t cb_alloc = cb_header + cb_elements;
        pBLdispatchers_t p  = (*ppdispatchers = (pBLdispatchers_t)malloc(cb_alloc));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->item_count = item_count;
        for (size_t i = 0; i < item_count; i++)
        {
            pBLdispatcher_core_t core = &p->dispatchers[i].core;
            core->handler = NULL;
            core->context = NULL;
            core->down_counter = core->down_counter_initial = 0;
        }
    } while (0);
    return err;
}

void BLdispatchers_delete(pBLdispatchers_t* ppdispatchers)
{
    pBLdispatchers_t dispatchers = *ppdispatchers;
    if (!dispatchers) return;
    for (size_t i = 0; i < dispatchers->item_count; i++)
    {
        BLdispatchers_clear(dispatchers, i);
    }
    free((void*)(*ppdispatchers));
    *ppdispatchers = NULL;
}

int BLdispatchers_set(pBLdispatchers_t dispatchers, size_t index, pcBLdispatcher_core_t settings)
{
    int err = EXIT_SUCCESS;
    do {
        if (index >= dispatchers->item_count)
        {
            err = ERANGE;
            break;
        }
        pBLdispatcher_t dispatcher = &dispatchers->dispatchers[index];
        if (EXIT_SUCCESS != (err = pthread_mutex_init(&dispatcher->mutex, NULL)))
        {
            break;
        }
        pBLdispatcher_core_t core = &dispatcher->core;
        core->down_counter = settings->down_counter;
        core->down_counter_initial = settings->down_counter_initial;
        core->context = settings->context;
        core->handler = settings->handler;
    } while (0);
    return err;
}

int BLdispatchers_reload(pBLdispatchers_t dispatchers, size_t index)
{
    int err = EXIT_SUCCESS;
    do {
        if (index >= dispatchers->item_count)
        {
            err = ERANGE;
            break;
        }
        pBLdispatcher_t dispatcher = &dispatchers->dispatchers[index];
        if (EXIT_SUCCESS != (err = pthread_mutex_lock(&dispatcher->mutex)))
        {
            break;
        }
        dispatcher->core.down_counter = dispatcher->core.down_counter_initial;
        pthread_mutex_unlock(&dispatcher->mutex);
    } while (0);
    return err;
}

int BLdispatchers_clear(pBLdispatchers_t dispatchers, size_t index)
{
    int err = EXIT_SUCCESS;
    do {
        if (index >= dispatchers->item_count)
        {
            err = ERANGE;
            break;
        }
        if (dispatchers->dispatchers[index].core.handler)
        {
            pBLdispatcher_t p = &dispatchers->dispatchers[index];
            pthread_mutex_destroy(&p->mutex);
            p->core.handler = NULL;
            p->core.down_counter = p->core.down_counter_initial = 0;
        }
    } while (0);
    return err;
}

static bool BLdispatcher_isdisabled(pcBLdispatcher_t dispatcher)
{
    return
        (dispatcher->core.handler == NULL) ||
        ((dispatcher->core.down_counter_initial == 0) && (dispatcher->core.down_counter == 0));
}

int BLdispatchers_dispatch(pBLdispatchers_t dispatchers)
{
    int err = EXIT_SUCCESS;
    do {
        for (size_t i = 0; i < dispatchers->item_count; i++)
        {
            pBLdispatcher_t dispatcher = &dispatchers->dispatchers[i];
            if (BLdispatcher_isdisabled(dispatcher))
            {
                continue;
            }
            pthread_mutex_lock(&dispatcher->mutex);
            (dispatcher->core.down_counter)--;
            if (0 == (dispatcher->core.down_counter))
            {
                dispatcher->core.handler(dispatcher->core.context);
                dispatcher->core.down_counter = dispatcher->core.down_counter_initial;
            }
            pthread_mutex_unlock(&dispatcher->mutex);
        }
    } while (0);
    return err;
}

static pBLdispatchers_t systick_dispatchers = NULL;
static BLsystick_t stcontext;

static void* dispatch_wrapper(void* param)
{
    pBLdispatchers_t dispatchers = (pBLdispatchers_t)param;
    int dispatch_result = BLdispatchers_dispatch(dispatchers);
    return (dispatch_result == EXIT_SUCCESS) ? param : NULL;
}

int BLsystickdispatcher_init(
    size_t dispatcher_count,
    pcBLdispatcher_core_t dispatchers,
    const struct timespec* systick_interval
) {
    int err = EXIT_SUCCESS;
    do {
        if (NULL != systick_dispatchers)
        { // already initialized
            err = EINVAL;
            break;
        }
        if (EXIT_SUCCESS != (err = BLdispatchers_new(&systick_dispatchers, dispatcher_count)))
        {
            break;
        }
        for (size_t i = 0; i < dispatcher_count; i++)
        {
            if (EXIT_SUCCESS != (err = BLdispatchers_set(systick_dispatchers, i, &dispatchers[i])))
            {
                break;
            }
        }
        if (err) break;
        BLsystick_acttime_setbytimespec(&stcontext.new, systick_interval);
        stcontext.systick_handler = dispatch_wrapper;
        stcontext.systick_args = systick_dispatchers;
        err = BLsystick_set(&stcontext);
    } while (0);
    return err;
}


void BLsystickdispatcher_destroy()
{
    BLsystick_restore(&stcontext);
    BLdispatchers_delete(&systick_dispatchers);
}

int BLsystickdispatcher_reload(size_t index)
{
    return BLdispatchers_reload(systick_dispatchers, index);
}