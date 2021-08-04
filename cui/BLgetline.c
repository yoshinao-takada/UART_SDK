#include "cui/BLgetline.h"
#include <memory.h>
#include <stdio.h>

pBLgetline_t BLgetline_new(int size)
{
    size_t allocsize = sizeof(BLgetline_t) + size;
    pBLgetline_t p = (pBLgetline_t)malloc(allocsize);
    if (!p)
    {
        return p;
    }
    p->pos = 0;
    p->size = size;
    memset(p->buf, 0, size);
    return p;
}

void BLgetline_clear(pBLgetline_t line)
{
    memset(line->buf, 0, line->size);
    line->pos = 0;
}

BLgetlineresult_t BLgetline_putc(pBLgetline_t line, int c)
{
    BLgetlineresult_t result = BLgetlineresult_normal;
    do {
        if ((' ' <= c) && (c <= '~')) // normal ASCII character
        {
            line->buf[line->pos++] = c;
            if (line->pos >= (line->size - 1))
            {
                result = BLgetlineresult_bufferfull;
            }
        }
        else if ((c == '\n') || (c == '\r')) // [ENTER] key
        {
            result = BLgetlineresult_EOL;
        }
        else if (c == 0x7f) // [DEL] (backspace key)
        { // clear the last character if it exists.
            if (line->pos)
            {
                line->pos--;
                line->buf[line->pos] = '\0';
            }
        }
        else if (c == 0x1b) // [ESC]
        { // clear input buffer
            line->pos = 0;
            memset(line->buf, '\0', line->size);
        }
    } while (0);
    return result;
}

pBLviewablegetline_t BLviewablegetline_new(int size, WINDOW* win, int y, int x)
{
    int err = EXIT_SUCCESS;
    pBLviewablegetline_t p = NULL;
    do {
        p = (pBLviewablegetline_t)malloc(sizeof(BLviewablegetline_t));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        if (NULL == (p->getline = BLgetline_new(size)))
        {
            err = ENOMEM;
            break;
        }
        p->win = win;
        p->x = x;
        p->y = y;
        p->on_eol = (BLcallbackctx_t){ NULL, NULL, NULL, NULL };
        p->on_bufferfull = (BLcallbackctx_t){ NULL, NULL, NULL, NULL };
        noecho();
        // keypad(stdscr, TRUE);
        // nodelay(stdscr, TRUE);
        // keypad(win->window, TRUE);
        // nodelay(win->window, TRUE);
    } while (0);
    if (err)
    {
        BLviewablegetline_delete(&p);
    }
    return p;
}

void BLviewablegetline_delete(pBLviewablegetline_t* ppobj)
{
    if (ppobj && *ppobj)
    {
        pBLviewablegetline_t pobj = *ppobj;
        BLSAFEFREE(&pobj->getline);
        BLSAFEFREE(ppobj);
    }
}

static void erase_region(WINDOW* w, int y0, int x0, int length)
{
    char* spaces = (char*)malloc(length + 1);
    memset(spaces, ' ', length);
    spaces[length] = '\0';
    wmove(w, y0, x0);
    waddstr(w, spaces);
    free(spaces);
}

static void refresh_region(WINDOW* w, int y0, int x0, pcBLgetline_t getline)
{
    erase_region(w, y0, x0, getline->size);
    wmove(w, y0, x0);
    waddstr(w, getline->buf);
    wmove(w, y0, x0 + getline->pos);
    wrefresh(w);
}

BLviewablegetlineresult_t  BLviewablegetline_getch(pBLviewablegetline_t obj)
{
    BLviewablegetlineresult_t result = BLviewablegetlineresult_none;
    BLgetlineresult_t getlineresult;
    int c;
    WINDOW* win = obj->win;
    pBLgetline_t getline = obj->getline;
    int cursorx = obj->x + getline->pos;
    int cursory = obj->y;
    do {
        wmove(win, cursory, cursorx);
        if (ERR == (c = wgetch(win)))
        {
            break;
        }
        getlineresult = BLgetline_putc(getline, c);
        refresh_region(win, obj->y, obj->x, getline);
        switch (getlineresult)
        {
            case BLgetlineresult_normal:
            {
                result = BLviewablegetlineresult_normal;
                break;
            }
            case BLgetlineresult_EOL:
            {
                result = BLviewablegetlineresult_EOL;
                // callback application specific EOL handler
                if (obj->on_eol.func)
                {
                    int eol_result = obj->on_eol.func(obj->on_eol.context, obj->on_eol.out, obj->on_eol.in);
                    if (eol_result != EXIT_SUCCESS)
                    {
                        result = BLviewablegetlineresult_error;
                    }
                }
                BLgetline_clear(getline);
                BLwin_clear(win);
                break;
            }
            case BLgetlineresult_bufferfull:
            {
                result = BLviewablegetlineresult_bufferfull;
                // callback application specific bufferfull handler
                if (obj->on_bufferfull.func)
                {
                    int bufferfull_result = obj->on_bufferfull.func(obj->on_bufferfull.context, obj->on_bufferfull.out, obj->on_bufferfull.in);
                    if (bufferfull_result != EXIT_SUCCESS)
                    {
                        result = BLviewablegetlineresult_error;
                    }
                }
                break;
            }
            default:
            {
                result = BLviewablegetlineresult_error;
                break;
            }
        }
        if (BLviewablegetlineresult_error == result)
        {
            break;
        }
        refresh_region(win, obj->y, obj->x, getline);
    } while (0);
    return result;
}