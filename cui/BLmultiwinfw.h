#ifndef BLMULTIWINFW_H_
#define BLMULTIWINFW_H_
/*!
\brief multi-window character user interface based on ncurses.
*/
#include "cui/BLmultiwin.h"
#include "cui/BLgetline.h"
#ifdef __cplusplus
extern "C" {
#endif
/*!
\brief initialize the framework
\param wincount [in] number of subwindows in a terminal window; e.g. ssh client, xterm, puTTY, etc.
\return unix errno compatible error code
*/
int BLmultiwinfw_init(int wincount, const char* logdir);

/*!
\brief initialize a subwindow
\brief 
*/
int BLmultiwinfw_initsub(int winid, const char* title, BLsubwin_loglevel_t loglevel, size_t format_buffer_size);

/*!
\brief get a formatting buffer
\param winid [in] subwindow ID
\param required_format_buffer_size [in] 0: no change is applied to the buffer size, != 0: apply realloc()
\return buffer pointer
*/
char* BLmultiwinfw_formatbuffer(int winid, size_t required_format_buffer_size);

/*!
\brief get a format buffer size
\param winid [in] subwindow ID
\return current buffer size
*/
size_t BLmultiwinfw_formatbuffersize(int winid);

/*!
\brief put a message in the format buffer onto the corresponding window screen.
*/
void BLmultiwinfw_transfer(int winid);

/*!
\brief put a message
*/
#define BLmultiwinfw_printf(winid, loglevel, ...) { \
    snprintf(BLmultiwinfw_formatbuffer(winid), BLmultiwinfw_formatbuffersize(int winid), __VA_ARGS__); \
    BLmultiwinfw_transfer(winid); }
#ifdef __cplusplus
}
#endif
#endif /* BLMULTIWINFW_H_ */