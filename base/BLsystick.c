#include    "base/BLsystick.h"
#include    <math.h>
#include    <memory.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <ucontext.h>
#pragma region BLsystick_acttime_setXXX
void BLsystick_acttime_setbytimeval(pBLsystick_acttime_t obj, const struct timeval* time)
{
    obj->t.it_interval.tv_sec = time->tv_sec;
    obj->t.it_interval.tv_usec = time->tv_usec;
    memcpy(&obj->t.it_value, &obj->t.it_interval, sizeof(struct timeval));
}

void BLsystick_acttime_setbytimespec(pBLsystick_acttime_t obj, const struct timespec* time)
{
    obj->t.it_interval.tv_sec = time->tv_sec;
    obj->t.it_interval.tv_usec = time->tv_nsec / 1000;
    memcpy(&obj->t.it_value, &obj->t.it_interval, sizeof(struct timeval));
}

void BLsystick_acttime_setbytvsec(pBLsystick_acttime_t obj, int tv_sec)
{
    obj->t.it_interval.tv_sec = tv_sec;
    obj->t.it_interval.tv_usec = 0;
    memcpy(&obj->t.it_value, &obj->t.it_interval, sizeof(struct timeval));
}

void BLsystick_acttime_setbyfloat(pBLsystick_acttime_t obj, float time)
{
    float int_part, fract_part;
    fract_part = modff(time, &int_part);
    obj->t.it_interval.tv_sec = (__time_t)int_part;
    obj->t.it_interval.tv_usec = (__suseconds_t)(fract_part * 1.0e6f);
    memcpy(&obj->t.it_value, &obj->t.it_interval, sizeof(struct timeval));
}

void BLsystick_acttime_setbydouble(pBLsystick_acttime_t obj, double time)
{
    double int_part, fract_part;
    fract_part = modf(time, &int_part);
    obj->t.it_interval.tv_sec = (__time_t)int_part;
    obj->t.it_interval.tv_usec = (__suseconds_t)(fract_part * 1.0e6);
    memcpy(&obj->t.it_value, &obj->t.it_interval, sizeof(struct timeval));
}
#pragma endregion BLsystick_acttime_setXXX

static BLcallback_t timer_callback = NULL;
static void* timer_context = NULL;

/*!
\brief hidden action handler encapsulating sigaction() API.
*/
static void my_handler(int signal_number)
{
    if (signal_number != SIGALRM)
    {
        return;
    }
    timer_callback(timer_context);
}

int BLsystick_set(pBLsystick_t systick)
{
    int err = EXIT_SUCCESS;
    do {
        systick->new.act.sa_flags = 0;
        sigemptyset(&systick->new.act.sa_mask);
        sigaddset(&systick->new.act.sa_mask, SIGALRM);
        systick->new.act.sa_handler = my_handler;
        systick->new.act.sa_flags = 0;
        timer_context = systick->systick_args;
        timer_callback = systick->systick_handler;
        if (-1 == sigaction(SIGALRM, &systick->new.act, &systick->old.act))
        {
            err = errno;
            break;
        }
        if (-1 == setitimer(ITIMER_REAL, &systick->new.t, &systick->old.t))
        {
            err = errno;
            sigaction(SIGALRM, &systick->old.act, &systick->new.act); // restore the old sigaction
            break;
        }
    } while (0);
    return err;
}

int BLsystick_restore(pBLsystick_t systick)
{
    int err = EXIT_SUCCESS;
    do {
        if (-1 == setitimer(ITIMER_REAL, &systick->old.t, &systick->new.t))
        {
            err = errno;
            break;
        }
        if (-1 == sigaction(SIGALRM, &systick->old.act, &systick->new.act))
        {
            err = errno;
            setitimer(ITIMER_REAL, &systick->new.t, &systick->old.t);
            break;
        }
    } while (0);
    return err;
}

void* BLsystick_samplecallback(void* param)
{
    pBLsystick_samplecontext_t context = (pBLsystick_samplecontext_t)param;
    int* param_count = &(context->count);
    fprintf(context->output, "%s,%d,count=%d\n", __FUNCTION__, __LINE__, *param_count);
    (*param_count)++;
    return param;
}