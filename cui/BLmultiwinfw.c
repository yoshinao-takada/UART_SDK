#include "cui/BLmultiwin.h"
#include "base/BLbase.h"
#include "cui/BLautobuf.h"
#include "cui/BLgetline.h"
#include <string.h>

typedef struct {
    BLautobuf_t buf;
    pBLsubwin_t win;
} BLtextwindow_t, *pBLtextwindow_t;
typedef const BLtextwindow_t *pcBLtextwindow_t;

// array of BLsubwin_t
typedef struct {
    int wincount;
    char* logdir;
    BLtextwindow_t textwindow[0];
} BLsubwindows_t, *pBLsubwindows_t;
typedef const BLsubwindows_t *pcBLsubwindows_t;

static pBLsubwindows_t subwindows = NULL;
static pBLgetline_t gBLgetline = NULL;

static int BLsubwindows_init(int wincount, const char* logdir)
{
    int err = EXIT_SUCCESS;
    do {
        size_t allocsize = sizeof(BLsubwindows_t) + wincount * sizeof(BLtextwindow_t) + strlen(logdir) + 1;
        subwindows = (pBLsubwindows_t)malloc(allocsize);
        subwindows->logdir = (char*)&subwindows->textwindow[wincount];
        strcpy(subwindows->logdir, logdir);
        for (int i = 0; i < wincount; i++)
        {
            subwindows->textwindow[i].buf.buf = NULL;
            subwindows->textwindow[i].buf.bufsize = 0;
            subwindows->textwindow[i].win = NULL;
        }
        subwindows->wincount = wincount;
        gBLgetline = BLgetline_new();
    } while (0);
    return err;
}

int BLmultiwinfw_init(int wincount, const char* logdir)
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = BLmultiwin_init(wincount)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = BLsubwindows_init(wincount, logdir)))
        {
            break;
        }
    } while (0);
    return err;
}

int BLmultiwinfw_initsub(int winid, const char* title, BLsubwin_loglevel_t loglevel, size_t format_buffer_size)
{
    int err = EXIT_SUCCESS;
    do {
        pBLtextwindow_t textwindow = &subwindows->textwindow[winid];
        if (NULL == BLautobuf_realloc(&textwindow->buf, format_buffer_size))
        {
            err = ENOMEM;
            break;
        }
        textwindow->win = BLmultiwin_get
        (winid, title, BLsubwin_timeformat_elapsed | BLsubwin_timeformat_sec_microsec, loglevel);
        if (textwindow->win == NULL)
        {
            err = EINVAL;
            break;
        }
    } while (0);
    return err;
}

char* BLmultiwinfw_formatbuffer(int winid, size_t required_format_buffer_size)
{
    assert(winid < subwindows->wincount);
    return (char*)subwindows->textwindow[winid].buf.buf;
}

size_t BLmultiwinfw_formatbuffersize(int winid)
{
    assert(winid < subwindows->wincount);
    return subwindows->textwindow[winid].buf.bufsize;
}

void BLmultiwinfw_transfer(int winid)
{
    assert(winid < subwindows->wincount);
    wprintw(subwindows->textwindow[winid].win->contents, "%s", (const char*)subwindows->textwindow[winid].buf.buf);
}