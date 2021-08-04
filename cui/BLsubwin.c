#include "cui/BLsubwin.h"
#include "base/BLbase.h"
#include "base/BLts.h"
#pragma region timestamp_formatter
/*!
\brief format time
\return rendered string length excluding terminating NULL character
*/
typedef int (*format_timestamp_with_mode)
(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize);

#pragma region format_only_functions
int timeformat_ISOtime(const struct timespec* time, char* buf, size_t bufsize)
{
    struct tm tm;
    gmtime_r(&time->tv_sec, &tm);
    return snprintf(buf, bufsize, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

int timeformat_ISOdatetime(const struct timespec* time, char* buf, size_t bufsize)
{
    struct tm tm;
    gmtime_r(&time->tv_sec, &tm);
    return snprintf(buf, bufsize, "%04d-%02d-%02d " "%02d:%02d:%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec);
}

int timeformat_sec(const struct timespec* time, char* buf, size_t bufsize)
{
    return snprintf(buf, bufsize, "%d", time->tv_sec);
}

int timeformat_sec_msec(const struct timespec* time, char* buf, size_t bufsize)
{
    return snprintf(buf, bufsize, "%d.%03d", time->tv_sec, time->tv_nsec/1000000);
}

int timeformat_sec_microsec(const struct timespec* time, char* buf, size_t bufsize)
{
    return snprintf(buf, bufsize, "%d.%06d", time->tv_sec, time->tv_nsec/1000);
}

int timeformat_sec_nanosec(const struct timespec* time, char* buf, size_t bufsize)
{
    return snprintf(buf, bufsize, "%d.%09d", time->tv_sec, time->tv_nsec);
}

int timeformat_none(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    return 0;
}
#pragma endregion format_only_functions
#pragma region elapsed_time_format_functions
int timeformat_elapsed_ISOtime(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    struct timespec time_to_render;
    BLts_ta_minus_tb(&time_to_render, time, origin);
    return timeformat_ISOtime(&time_to_render, buf, bufsize);
}

int timeformat_elapsed_ISOdatetime(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    struct timespec time_to_render;
    BLts_ta_minus_tb(&time_to_render, time, origin);
    return timeformat_ISOdatetime(&time_to_render, buf, bufsize);
}

int timeformat_elapsed_sec(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    struct timespec time_to_render;
    BLts_ta_minus_tb(&time_to_render, time, origin);
    return timeformat_sec(&time_to_render, buf, bufsize);
}

int timeformat_elapsed_sec_msec(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    struct timespec time_to_render;
    BLts_ta_minus_tb(&time_to_render, time, origin);
    return timeformat_sec_msec(&time_to_render, buf, bufsize);
}

int timeformat_elapsed_sec_microsec(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    struct timespec time_to_render;
    BLts_ta_minus_tb(&time_to_render, time, origin);
    return timeformat_sec_microsec(&time_to_render, buf, bufsize);
}

int timeformat_elapsed_sec_nanosec(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    struct timespec time_to_render;
    BLts_ta_minus_tb(&time_to_render, time, origin);
    return timeformat_sec_nanosec(&time_to_render, buf, bufsize);
}
#pragma endregion elapsed_time_format_functions
#pragma region absolute_time_format_functions
int timeformat_absolute_ISOtime(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    return timeformat_ISOtime(time, buf, bufsize);
}

int timeformat_absolute_ISOdatetime(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    return timeformat_ISOdatetime(time, buf, bufsize);
}

int timeformat_absolute_sec(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    return timeformat_sec(time, buf, bufsize);
}

int timeformat_absolute_sec_msec(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    return timeformat_sec_msec(time, buf, bufsize);
}

int timeformat_absolute_sec_microsec(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    return timeformat_sec_microsec(time, buf, bufsize);
}

int timeformat_absolute_sec_nanosec(const struct timespec* time, const struct timespec* origin, char* buf, size_t bufsize)
{
    return timeformat_sec_nanosec(time, buf, bufsize);
}
#pragma endregion absolute_time_format_functions

const format_timestamp_with_mode formatters[3][6] =
{
    { timeformat_none, timeformat_none, timeformat_none, timeformat_none, timeformat_none, timeformat_none },
    {
        timeformat_elapsed_ISOtime, timeformat_elapsed_ISOdatetime, timeformat_elapsed_sec,
        timeformat_elapsed_sec_msec, timeformat_elapsed_sec_microsec, timeformat_elapsed_sec_nanosec
    },
    {
        timeformat_absolute_ISOtime, timeformat_absolute_ISOdatetime, timeformat_absolute_sec,
        timeformat_absolute_sec_msec, timeformat_absolute_sec_microsec, timeformat_absolute_sec_nanosec
    }
};
#define TIMESTAMP_LENGTH    21  /* max(strlen("yyyy-MM-dd hh:mm:ss,"), digits(300 years in sec + 10 digit frac part) + 1 */
const char* loglevel_labels[] =
{
    "none", "error", "warning", "info", "debug", "trace"
};
#define LOGLEVEL_LENGTH     8   /* strlen("warning") + strlen(",") */
#define LOGBUFFER_SIZE      32 /* LOGBUFFER_SIZE >= TIMESTAMP_LENGTH + LOGLEVEL_LENGTH */
#pragma endregion timestamp_formatter

void BLsubwin_puts(pBLsubwin_t win, BLsubwin_loglevel_t loglevel, const char* message)
{
    wprintw(win->contents, "%s", message);
    wrefresh(win->contents);
    if (win->logsink && (loglevel <= win->loglevel))
    {
        int basic_mode_index = (win->timeformat & 0xf0) >> 4;
        int format_index = win->timeformat & 0x0f;
        format_timestamp_with_mode formatter = formatters[basic_mode_index][format_index];
        struct timespec ts;
        timespec_get(&ts, TIME_UTC);
        {
            char logbuffer[LOGBUFFER_SIZE];
            int  length = formatter(&ts, &win->origin, logbuffer, win->width);
            snprintf(logbuffer + length, LOGBUFFER_SIZE - length, ",%s", loglevel_labels[loglevel]);
            fprintf(win->logsink, "%s,%s", logbuffer, message);
        }
    }
}

