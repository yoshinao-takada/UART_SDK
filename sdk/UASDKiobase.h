#ifndef UASDKIOBASE_H_
#define UASDKIOBASE_H_
#include    <termios.h>
#include    <stdint.h>
#include    <time.h>
#include    <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif
#pragma region baudrate
typedef enum {
    UASDKbyteformat_N1, // no parity, 1 stop bit
    UASDKbyteformat_E1, // even parity, 1 stop bit
    UASDKbyteformat_O1, // odd parity, 1 stop bit
    UASDKbyteformat_N2, // no parity, 2 stop bit
    UASDKbyteformat_E2, // even parity, 2 stop bit
    UASDKbyteformat_O2, // odd parity, 2 stop bit
} UASDKbyteformat_t;

// this macro defines an instance of UASDKbaudrate_t.
// e.g. UASDKBAUD(1200) ==> { B1200, 1200, (1.0f/1200.0f) }
#define UASDKBAUD(BRNUM) {B ## BRNUM , BRNUM , (1.0f/ BRNUM ## .0f)}

typedef struct {
    int id; // identifier defined in unix header; e.g. B9600, B1200, etc.
    int bps; // baudrate represented by bits/sec
    float bittime; // period of 1 bit in represented by seconds
} UASDKbaudrate_t, *pUASDKbaudrate_t;
typedef const UASDKbaudrate_t *pcUASDKbaudrate_t;

/*!
\brief estimate time for transferring the specified bytes.
\param baudate [in] baudrate
\param byte_count [in]
\param format [in] byte format
\param time [out] estimate of the time
*/
void UASDKbaudrate_estimatetime(pcUASDKbaudrate_t baudrate, int byte_count, UASDKbyteformat_t format, struct timespec* time);
#pragma endregion baudrate
#pragma region unified_buffer

// void* buffer transferred to standard C APIs
typedef struct {
    void* buf;
    size_t byte_count;
    size_t filled_bytes;
} UASDKvoidbuf_t, *pUASDKvoidbuf_t;
typedef const UASDKvoidbuf_t *pcUASDKvoidbuf_t;

// byte buffer for byte-by-byte access
typedef struct {
    uint8_t* buf;
    size_t byte_count;
    size_t filled_bytes;
} UASDKbytebuf_t, *pUASDKbytebuf_t;
typedef const UASDKbytebuf_t *pcUASDKbytebuf_t;

// unified buffer
typedef union {
    UASDKvoidbuf_t voidbuf;
    UASDKbytebuf_t bytebuf;
} UASDKunibuf_t, *pUASDKunibuf_t;
typedef const UASDKunibuf_t *pcUASDKunibuf_t;

#define UASDKunibuf_byte_count(ub) ub->bytebuf.byte_count
#define UASDKunibuf_void(ub)    ub->voidbuf.buf
#define UASDKunibuf_byte(ub)    ub->bytebuf.buf
#define UASDKunibuf_initdef    { { NULL, 0, 0 } }

/*!
\brief initialize as a byte buffer
\param ub [in,out] object to initialize
\param total_byte_count [in] total byte count of byte_count.buf
\return unix errno compatible number
*/
int UASDKunibuf_initbyte(pUASDKunibuf_t ub, uint16_t total_byte_count);

/*!
\brief extract specified bytes
\param ub [in,out] unified buffer object
\param ext_bytes [in] byte count to extract.
\param outbuf [out] output buffer
\return unix errno compatible number
*/
int UASDKunibuf_extract(pUASDKunibuf_t ub, uint16_t ext_bytes, void* outbuf);

/*!
\brief clean up allocated memory block
\param ub [in,out] object to clean up
\return unix errno compatible number
*/
void UASDKunibuf_destroy(pUASDKunibuf_t ub);
#pragma endregion unified_buffer
#pragma region init_uart
typedef struct {
    UASDKbaudrate_t baudrate;
    UASDKbyteformat_t byteformat;
    int fd;
} UASDKuartdescriptor_t, *pUASDKuartdescriptor_t;
typedef const UASDKuartdescriptor_t *pcUASDKuartdescriptor_t;
#define UASDKUARTDEF0 { UASDKBAUD(9600), UASDKbyteformat_N1, -1 }

/*!
\brief open serialport
\param uart [out] UART descriptor
\param name [in] device name
\param br [in] baudrate
\param bf [in] byte format; currently not supported, reserved; current byte format is only 8N1.
\return errno compatible code
*/
int UASDKiobase_open(pUASDKuartdescriptor_t uart, const char* name, pcUASDKbaudrate_t br, UASDKbyteformat_t bf);

/*!
\brief write data
\param uart [in] UART descriptor
\param ub [in] unified buffer
\param actually_written [out] actually written byte count
\return unix errno compatible number
*/
int UASDKiobase_write(pcUASDKuartdescriptor_t uart, pcUASDKunibuf_t ub, int* actually_written);

/*!
\brief read data
\param uart [in] UART descriptor
\param ub [in] unified buffer
\return unix errno compatible number
*/
int UASDKiobase_read(pcUASDKuartdescriptor_t uart, pUASDKunibuf_t ub);

int UASDKiobase_enableinterrupt(int signum);
#pragma endregion init_uart
#ifdef __cplusplus
}
#endif
#endif /* UASDKIOBASE_H_ */