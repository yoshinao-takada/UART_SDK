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
\brief get default setup of a serial port (normal 9600 BPS, HS 115200 BPS)
\return the pointer to the internal static variable of the default setup
*/
pcUASDK_setup_t UASDK_default_setup(); // 9600 BPS
pcUASDK_setup_t UASDK_default_setupHS(); // 115200 BPS

/*!
\brief estimate time period to transfer bytes.
\param setup [in] contains baudrate bit period.
\param byte_count [in] to transfer.
*/
void UASDK_setup_estimate_time(pcUASDK_setup_t setup, int byte_count, struct timespec* time);

#pragma region UASDK_commport_t
typedef struct {
    int fd; // file descriptor of comm port
    char* name; // comm port name
    UASDK_setup_t setup; // port setup, baudrate and byte format
    uint8_t buf[0]; // head of extra memory block
} UASDK_commport_t, *pUASDK_commport_t;
typedef const UASDK_commport_t pcUASDK_commport_t;

/*!
\brief open and configure commport
\param name [in] device name e.g. /dev/ttyS0, /dev/ttyUSB1
\param setup [in] baudrate and byte format
\param ppcommport [out] newly opened object
\return errno
*/
int UASDK_commport_open(const char* name, pcUASDK_setup_t setup, pUASDK_commport_t *ppcommport);

/*!
\brief close commport and release the allocated resources
\param ppcommport [in,out] the object to destroy
\return errno
*/
int UASDK_commport_close(pUASDK_commport_t *ppcommport);

/*!
\brief send break signal and wait (the break signal time + marginal time)

*/
#pragma endregion UASDK_commport_t

#ifdef __cplusplus
}
#endif

#endif /* UASDK_SETUP_H_ */
