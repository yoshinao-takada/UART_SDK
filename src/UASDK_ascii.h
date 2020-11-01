#ifndef UASDK_ASCII_H_
#define UASDK_ASCII_H_
/*!
\file UASDK_ascii.h
\brief ASCII packet transceiver
*/
#include    "UASDK_buffer.h"
#include    "UASDK_setup.h"

#ifdef __cplusplus
extern "C" {
#endif

// status flags in UASDK_ascii_read() return number
#define UASDK_ascii_rlen_sts    0x030000  /* received length status mask, bit16,17 */
#define UASDK_ascii_rlen_sts_nodata 0
#define UASDK_ascii_rlen_sts_normal 0x010000 /* bit16=1, bit17=0 */
#define UASDK_ascii_rlen_sts_bufferfull 0x020000 /* bit16=0, bit17=1 */
#define UASDK_ascii_rnc_sts     0x0c0000 /* received null character status mask, bit18,19 */
#define UASDK_ascii_rnc_sts_0   0x000000 /* no null character was received. */
#define UASDK_ascii_rnc_sts_1   0x040000 /* 1 or more null character was recieved with other characters */

/*!
\brief try to read a serialport
\param fd [in] file descriptor of the serialport
\param buffer [in,out] read buffer
\return packed int number of error code (bit 0..15), status flags (bit 16..31)
*/
int UASDK_ascii_read(int fd, pUASDK_buffer_t buffer);

/*!
\brief try to write a serialport
\param fd [in] file descriptor of the serialport
\param buffer [in,out] read buffer
\param setup [in] communication interface setup
\return errno
*/
int UASDK_ascii_write(int fd, pcUASDK_buffer_t buffer, pcUASDK_setup_t setup);

/*!
\brief get the length of the first string in the buffer. The length does NOT include the 1st null character.
Return buffer->caplen.byte_filled if no null character exists in the buffer.
\param buffer [in]
\return string length
*/
int UASDK_ascii_1st_strlen(pcUASDK_buffer_t buffer);

/*!
\brief shift out a string from buffer head to the 1st null character if the string exists.
    NULL termination is appended if the string in 'in' is not terminated by NULL.
\param in [in,out] input buffer
\param out [out] output buffer
\return errno
*/
int UASDK_ascii_shiftout(pUASDK_buffer_t in, pUASDK_buffer_t out);
#ifdef __cplusplus
}
#endif
#endif /* UASDK_ASCII_H_ */