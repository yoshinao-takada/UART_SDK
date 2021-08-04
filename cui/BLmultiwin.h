#ifndef BLMULTIWIN_H_
#define BLMULTIWIN_H_
#include <ncurses.h>
#include "cui/BLsubwin.h"
#ifdef __cplusplus
extern "C" {
#endif

/*!
\brief initialize multi-window context
\return error code
*/
int BLmultiwin_init(int wincount);

/*!
\brief destropy multi-window context
*/
int BLmultiwin_destroy();

/*!
\brief get subwindow object
\param subwin_index [in]
\param timeformat [in] timestamp format
\param loglevel [in] loglevel
\return NULL or subwindow pointer
*/
pBLsubwin_t BLmultiwin_get(
    int subwin_index,
    const char* title,
    BLsubwin_timeformat_t timeformat,
    BLsubwin_loglevel_t loglevel
);

/*!
\brief release subwindow object
\param subwin
\return EXIT_SUCCESS or EINVAL
*/
int BLmultiwin_release(pBLsubwin_t subwin);

/*!
\brief get window height of the main window
*/
int BLmultiwin_getheight();

/*!
\brief get window width of the main window
*/
int BLmultiwin_getwidth();
#ifdef __cplusplus
}
#endif
#endif /* BLMULTIWIN_H_ */