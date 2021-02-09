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

#define UASDK_buffer_vacant_begin(buffer) (buffer->head.pbytes + buffer->caplen.byte_filled)
#define UASDK_buffer_filled_begin(buffer) (buffer->caplen.byte_filled ? buffer->head.pbytes : (uint8_t*)NULL)

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

typedef enum {
    UASDK_bufcpresult_fullsuccess, // all of required bytes are copied.
    UASDK_bufcpresult_lessbysrc, // source buffer has less bytes than request, all of source bytes were copied.
    UASDK_bufcpresult_lessbydst, // destination buffer has less capacity than request, capacity bytes were copied.
    UASDK_bufcpresult_error, // error, e.g. NULL pointer,
} UASDK_bufcpresult_t;
/*!
\brief copy specified bytes
\param buffer_dst [out] copy destination
\param buffer_src [in,out] copy source
\param byte_count [in,out] in: request, out: actually copied
\return result code
*/
UASDK_bufcpresult_t UASDK_buffer_copy(pUASDK_buffer_t dst, pUASDK_buffer_t src, uint8_t *byte_count);

/*!
\brief shift-out buffer contents without error check
\param dst [out] pointer to the destination memory block
\param src [in,out] copy source
\param byte_count [in] byte count to shift-out
\return byte count actually shifted out
*/
uint8_t UASDK_buffer_shiftout(void* dst, pUASDK_buffer_t src, uint8_t byte_count);
#ifdef __cplusplus
}
#endif

#endif /*UASDK_BUFFER_H_ */