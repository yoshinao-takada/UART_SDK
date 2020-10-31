#ifndef UASDK_BUFFER_H_
#define UASDK_BUFFER_H_
#include    <stdint.h>
#include    <UASDK_defs.h>
#include    <wchar.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    union {
        uint8_t *pbytes;
        uint16_t *pword;
        char* pstring;
        wchar_t* pwstring;
    } head;
    UASDK_caplen_t caplen;
    uint8_t buf[0]; // raw buffer region
} UASDK_buffer_t, *pUASDK_buffer_t;
typedef const UASDK_buffer_t *pcUASDK_buffer_t;

/*!
\brief create a new buffer object
\param capacity [in] The capacity of the buffer counted in bytes
\param ppbuffer [out] pointer pointer to the buffer object
\return errno
*/
int UASDK_buffer_new(uint8_t capacity, pUASDK_buffer_t *ppbuffer);

/*!
\brief clear the buffer
\param buffer [in,out] The buffer object to clear.
*/
void UASDK_buffer_clear(pUASDK_buffer_t buffer);

#ifdef __cplusplus
}
#endif

#endif /*UASDK_BUFFER_H_ */