#ifndef UASDK_DEFS_H_
#define UASDK_DEFS_H_
#include    <stdint.h>
#include    <termios.h>

#ifdef  __cplusplus
extern "C" {
#endif
/*!
\struct UASDK_caplen_t
\brief capacity and length of buffer
*/
typedef struct {
    uint8_t byte_filled; // length counted in bytes
    uint8_t byte_capacity; // buffer capacity counted in bytes
} UASDK_caplen_t, *pUASDK_caplen_t;
typedef const UASDK_caplen_t *pcUASDK_caplen_t;

#define UASDK_CAPLEN_AVAILABLE(caplen) ((int)(caplen.byte_capacity - caplen.byte_filled))

typedef struct {
    int id; // baudrate identifier in linux header,e.g. B9600, B57400
    int baud; // baudrate
    float bit_period; // time period of 1 bit transfer
} UASDK_baudrate_t, *pUASDK_baudrate_t;
typedef const UASDK_baudrate_t *pcUASDK_baudrate_t;

// this macro defines an instance of UASDK_baudrate_t.
// e.g. UASDK_BAUD(1200) ==> { B1200, 1200, (1.0f/1200.0f) }
#define UASDK_BAUD(BRNUM) {B ## BRNUM , BRNUM , (1.0f/ BRNUM ## .0f)}

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)    sizeof(a)/sizeof(a[0])
#endif

#ifdef  __cplusplus
}
#endif
#endif /* UASDK_DEFS_H_ */