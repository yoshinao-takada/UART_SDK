#ifndef BLGETLINLE_H_
#define BLGETLINLE_H_
#include "base/BLbase.h"
#include <ncurses.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    int pos;
    int size;
    char buf[0];
} BLgetline_t, *pBLgetline_t;
typedef const BLgetline_t *pcBLgetline_t;

/*!
\brief create a new object
*/
pBLgetline_t BLgetline_new(int size);

typedef enum {
    BLgetlineresult_normal,
    BLgetlineresult_bufferfull,
    BLgetlineresult_EOL
} BLgetlineresult_t;

/*!
\brief simple getline buffer, accepting only
1) character keys (alphanumerics and special marks like +,-,*,/,(,[,<...)
2) [DEL](0x7f) (backspace key generate [DEL] code)
3) carriage return or line feed (end of line)
4) [ESC](0x1b) (clear buffer)
*/
BLgetlineresult_t BLgetline_putc(pBLgetline_t line, int c);

/*!
\brief clear buffer
*/
void BLgetline_clear(pBLgetline_t line);

typedef struct {
    WINDOW* win; // viewable
    pBLgetline_t getline; // getline object
    int y, x; // base position relative to viewable window
    BLcallbackctx_t on_eol;
    BLcallbackctx_t on_bufferfull;
} BLviewablegetline_t, *pBLviewablegetline_t;
typedef const BLviewablegetline_t *pcBLviewablegetline_t;

/*!
\brief create viewable getline object
\param size [in] buffer size of getline
\param win [in] window object
\param y, x [in] base position relative to viewable window
\return newly created object
*/
pBLviewablegetline_t BLviewablegetline_new(int size, WINDOW* win, int y, int x);

/*!
\brief delete an object
*/
void BLviewablegetline_delete(pBLviewablegetline_t* ppobj);

typedef enum {
    BLviewablegetlineresult_none,
    BLviewablegetlineresult_normal,
    BLviewablegetlineresult_bufferfull,
    BLviewablegetlineresult_EOL,
    BLviewablegetlineresult_error,
} BLviewablegetlineresult_t;

/*!
\brief get a keyboard input
*/
BLviewablegetlineresult_t  BLviewablegetline_getch(pBLviewablegetline_t obj);

void BLgetline_setlogsink(FILE* pf);
#ifdef __cplusplus
}
#endif
#endif /* BLGETLINLE_H_ */