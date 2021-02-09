#include    <time.h>

void BLts_getnow(struct timespec* t)
{
    timespec_get(t, TIME_UTC);
}

void BLts_ta_minus_tb(struct timespec* ta_minus_tb, const struct timespec* ta, const struct timespec* tb)
{
    __syscall_slong_t borrow = 0;
    ta_minus_tb->tv_sec = ta->tv_sec - tb->tv_sec;
    if (ta->tv_nsec < tb->tv_nsec)
    {
        borrow = 1000000000;
        ta_minus_tb->tv_sec--;       
    }
    ta_minus_tb->tv_nsec = ta->tv_nsec + (borrow - tb->tv_nsec);
}

void BLts_getelapsed(struct timespec* telapsed, const struct timespec* tref)
{
    struct timespec tnow;
    BLts_getnow(&tnow);
    BLts_ta_minus_tb(telapsed, &tnow, tref);
}

float BLts_tofs(const struct timespec* t)
{
    return (float)t->tv_sec + 1.0e-9f * (float)t->tv_nsec;
}

float BLts_tofms(const struct timespec* t)
{
    return 1000.0f * BLts_tofs(t);
}
