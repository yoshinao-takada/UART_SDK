#ifndef UASDK_SETUP_H_
#define UASDK_SETUP_H_
#include    "UASDK_defs.h"
#include    <time.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    UASDK_byteformat_N1, // no parity, 1 stop bit
    UASDK_byteformat_E1, // even parity, 1 stop bit
    UASDK_byteformat_O1, // odd parity, 1 stop bit
    UASDK_byteformat_N2, // no parity, 2 stop bit
    UASDK_byteformat_E2, // even parity, 2 stop bit
    UASDK_byteformat_O2, // odd parity, 2 stop bit
} UASDK_byteformat_t;

typedef struct {
    UASDK_baudrate_t baudrate;
    UASDK_byteformat_t byteformat;    
} UASDK_setup_t, *pUASDK_setup_t;

typedef const UASDK_setup_t *pcUASDK_setup_t;

#define UASDK_SETUP_DEFAULT { UASDK_BAUD(9600), UASDK_byteformat_N1 };
#define UASDK_SETUP_HSDEFAULT { UASDK_BAUD(115200), UASDK_byteformat_N1 };

/*!
\brief set communication mode of a serial interface
\param setup [in] setup parameters
\fd [in] file descriptor of the serial interface device
\return errno
*/
int UASDK_setup(pcUASDK_setup_t setup, int fd);

/*!
\brief open serial communication port
\param devicename [in] full path name of serialport device like "/dev/ttyUSB0", "/dev/ttyS0", etc.
\param pfd [out] file descriptor of the opened device
\return errno
*/
int UASDK_open(const char* devicename, int* pfd);

/*!
\brief get default setup of a serial port
\return the pointer to the internal static variable of the default setup
*/
pcUASDK_setup_t UASDK_default_setup();

/*!
\brief estimate time period to transfer bytes.
\param setup [in] contains baudrate bit period.
\param byte_count [in] to transfer.
*/
void UASDK_setup_estimate_time(pcUASDK_setup_t setup, int byte_count, struct timespec* time);
#ifdef __cplusplus
}
#endif

#endif /* UASDK_SETUP_H_ */
