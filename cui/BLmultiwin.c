#include "cui/BLmultiwin.h"
#include "base/BLbase.h"
#include "base/BLfile.h"

typedef enum {
    FwhiteBblack = 1, // foreground: white, background: black
    FblackBwhite = 2 // foreground: black, background: white
} BLcolors_t;

typedef struct {
    int count;
    WINDOW* main_window;
    WINDOW* footer;
    BLsubwin_t sub[0];
} BLmultiwin_t, *pBLmultiwin_t;

static pBLmultiwin_t display = NULL;

int* calc_sub_window_heights(int wincount, int main_window_height)
{
    main_window_height--; // reduce footer height
    int minimum_height = main_window_height / wincount;
    if (minimum_height < 5)
    {
        return NULL;
    }
    int* heights = (int*)calloc(wincount, sizeof(int));
    int residual = main_window_height - minimum_height * wincount;
    for (int i = 0; i < wincount; i++)
    {
        heights[i] = minimum_height;
        if (residual)
        {
            heights[i]++;
            residual--;
        }
    }
    return heights;
}

int BLmultiwin_init(int wincount)
{
    int err = EXIT_SUCCESS;
    int* sub_window_heights;
    struct timespec origin;
    timespec_get(&origin, TIME_UTC);
    int title_row = 0;
    int cx, cy;
    do {
        if (display)
        {
            err = EEXIST;
            break;
        }
        if (NULL == (display = (pBLmultiwin_t)malloc(sizeof(BLmultiwin_t) + wincount * sizeof(BLsubwin_t))))
        {
            err = ENOMEM;
            break;
        }
        display->count = wincount;
        display->main_window = initscr();
        getmaxyx(display->main_window, cx, cy);
        start_color();
        init_pair(FwhiteBblack, COLOR_WHITE, COLOR_BLACK);
        init_pair(FblackBwhite, COLOR_BLACK, COLOR_WHITE);
        int main_window_height;
        int width;
        getmaxyx(display->main_window, main_window_height, width);
        if (NULL == (sub_window_heights = calc_sub_window_heights(wincount, main_window_height)))
        {
            err = EINVAL;
            break;
        }
        for (int i = 0; i < wincount; i++)
        {
            display->sub[i].titlebar = subwin(display->main_window, 1, width, title_row, 0);
            display->sub[i].contents = subwin(display->main_window, sub_window_heights[i] - 1, width, title_row + 1, 0);
            wbkgd(display->sub[i].titlebar, COLOR_PAIR(FblackBwhite));
            scrollok(display->sub[i].contents, TRUE);
            wclear(display->sub[i].titlebar);
            wrefresh(display->sub[i].titlebar);
            wclear(display->sub[i].contents);
            wrefresh(display->sub[i].contents);
            title_row += sub_window_heights[i];
            display->sub[i].logsink = NULL;
            display->sub[i].width = width;
            display->sub[i].height = sub_window_heights[i] - 1;
            display->sub[i].origin = origin;
            display->sub[i].timeformat = BLsubwin_timeformat_none | BLsubwin_timeformat_sec;
            display->sub[i].loglevel = BLsubwin_loglevel_none;
            display->sub[i].message_buffer.bufsize = cx;
            display->sub[i].message_buffer.buf = malloc(cx);
        }
        {
            display->footer = subwin(display->main_window, 1, width, title_row, 0);
            wbkgd(display->footer, COLOR_PAIR(FblackBwhite));
            wclear(display->footer);
            wrefresh(display->footer);
        }
    } while(0);
    BLSAFEFREE(&sub_window_heights);
    return err;
}

int BLmultiwin_destroy()
{
    int err = EXIT_SUCCESS;
    do {
        if (!display)
        {
            err = ENODATA;
            break;
        }
        for (int i = 0; i < display->count; i++)
        {
            BLSAFEFREE(&display->sub[i].message_buffer.buf);
            display->sub[i].message_buffer.bufsize = 0;
        }
        endwin();
        BLSAFEFREE(&display);
    } while (0);
    return err;
}

pBLsubwin_t BLmultiwin_get(
    int subwin_index,
    const char* title,
    BLsubwin_timeformat_t timeformat,
    BLsubwin_loglevel_t loglevel
) {
    fprintf(stderr, "%s, %d, %s\n", __FUNCTION__, subwin_index, title);
    pBLsubwin_t subwindow = NULL;
    int err = EXIT_SUCCESS;
    do {
        if (subwin_index >= display->count)
        {
            break;
        }
        subwindow = &display->sub[subwin_index];
        subwindow->timeformat = timeformat;
        subwindow->loglevel = loglevel;
        if (title)
        {
            size_t alloc_size = strlen(title) + strlen(".log") + 1;
            char* pathbuffer = (char*)malloc(alloc_size);
            snprintf(pathbuffer, alloc_size, "%s%s", title, ".log");
            if (BLfile_open(&subwindow->logsink, pathbuffer, "w"))
            {
                subwindow = NULL;
                BLSAFEFREE(&pathbuffer);
                break;
            }
            BLSAFEFREE(&pathbuffer);
            wclear(subwindow->titlebar);
            wmove(subwindow->titlebar, 0, 0);
            waddstr(subwindow->titlebar, title);
            wrefresh(subwindow->titlebar);
        }
    } while (0);
    return subwindow;
}

int BLmultiwin_release(pBLsubwin_t subwindow)
{
    int err = EXIT_FAILURE;
    for (int i = 0; i < display->count; i++)
    {
        if (&display->sub[i] == subwindow)
        {
            BLSAFEFREE(&subwindow->message_buffer.buf);
            subwindow->message_buffer.bufsize = 0L;
            if (subwindow->logsink)
            {
                fclose(subwindow->logsink);
                subwindow->logsink = NULL;
            }
            err = EXIT_SUCCESS;
            break;
        }
    }
    return err;
}

int BLmultiwin_getheight()
{
    int y, x;
    getmaxyx(display->main_window, y, x);
    return y;
}

int BLmultiwin_getwidth()
{
    int y, x;
    getmaxyx(display->main_window, y, x);
    return x;
}
