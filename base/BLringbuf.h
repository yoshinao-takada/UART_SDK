#ifndef BLRINGBUF_H_
#define BLRINGBUF_H_
#ifdef __cplusplus
#include    <cstdint>
extern "C" {
#else
#include    <stdint.h>
#endif
#include    <pthread.h>
#pragma region ring_buffer_without_exclusive_control
typedef struct {
    uint16_t mask;
    uint16_t wrptr;
    uint16_t rdptr;
    uint8_t buffer[0];
} BLringbuf_t, *pBLringbuf_t;
typedef const BLringbuf_t *pcBLringbuf_t;

/*!
\brief create a new ring buffer object
\param count_bits [in] 1..16, effective bit count of mask, wrptr, rdptr.  buffer size
is 2^(count_bits - 1).
\param ppobj [out]
\return errno compatible number
*/
int BLringbuf_new(uint8_t count_bits, pBLringbuf_t* ppobj);

/*!
\brief put byte string
\param obj [in,out] ring buffer object
\param byte_count [in] byte count of data
\param data [in] data to put
\param actual [out] actually put byte count
\return ENOBUFS: Not all data was put because buffer has insufficient space,
    otherwise EXIT_SUCCESS.
*/
int BLringbuf_put(pBLringbuf_t obj, uint16_t byte_count, const uint8_t* data, uint16_t* actual);
/*!
\brief pub byte string. Old contents are discarded if the buffer is full.
\param obj [in,out] ring buffer object
\param byte_count [in] byte count of data
\param data [in] data to put
\return ENOBUFS: any parts of old contents are discarded. Otherwise EXIT_SUCCESS.
*/
int BLringbuf_putforce(pBLringbuf_t obj, uint16_t byte_count, const uint8_t* data);

/*!
\brief get byte string and update rdptr
\param obj [in,out] ring buffer object
\param byte_count [in] requested byte count
\param data [out] buffer where gotten data is put
\param actual [out] actually gotten byte count
\return ENODATA: Not all data was gotten because buffer has sufficient data,
    otherwise EXIT_SUCCESS.
*/
int BLringbuf_get(pBLringbuf_t obj, uint16_t byte_count, uint8_t* data, uint16_t* actual);

/*!
\brief get byte string and do not update rdptr
\param obj [in,out] ring buffer object
\param byte_count [in] requested byte count
\param data [out] buffer where gotten data is put
\param actual [out] actually gotten byte count
\return ENODATA: Not all data was gotten because buffer has sufficient data,
    otherwise EXIT_SUCCESS.
*/
int BLringbuf_peek(pBLringbuf_t obj, uint16_t byte_count, uint8_t* data, uint16_t* actual);

/*!
\brief move wrptr ahead. It does not check available space.
\param obj [in,out] ring buffer object
\param offset [in] requested byte count
*/
void BLringbuf_wrptr_goahead(pBLringbuf_t obj, uint16_t offset);

/*!
\brief move rdptr ahead. It does not check available data.
\param obj [in,out] ring buffer object
\param offset [in] requested byte count
*/
void BLringbuf_rdptr_goahead(pBLringbuf_t obj, uint16_t offset);

/*!
\brief get current available space
\param obj [in] ring buffer object
\return current availabel space byte count
*/
uint16_t BLringbuf_available_space(pcBLringbuf_t obj);

/*!
\brief get current avalable data byte count
\param obj [in] ring buffer object
\return current availabel data byte count
*/
uint16_t BLringbuf_available_data(pcBLringbuf_t obj);

/*!
\brief clear all
\param obj [in,out] ring buffer object
*/
void BLringbuf_clear(pBLringbuf_t obj);

/*!
\brief treat the ring buffer as a pointer queue.
\return EXIT_SUCCESS: success, ENOSPC: insufficient space and no change is applied to obj.
*/
int BLringbuf_putptr(pBLringbuf_t obj, void* pv);

/*!
\brief treat the ring buffer as a pointer queue
\return NULL: no-data, valid pointer: success
*/
void* BLringbuf_getptr(pBLringbuf_t obj);

/*!
\brief clear the ring buffer and all contents.
The contents are regarded as pointers simply allocated by malloc.
All the content pointers are freed by free(BLringbuf_getptr(obj));
*/
void BLringbuf_clearptr(pBLringbuf_t obj);

/*!
\brief get a size index which is sufficient to hold the number of pointers.
\param count [in] number of elements which can be held in the buffer.
\param element_size [in] size of each element
\return size index for count_bits of BLringbuf_new(). If (count * element_size) is too large, return 0.
*/
uint8_t BLringbuf_sizeindex(uint16_t count, uint16_t element_size);

// size index calculator for (void*) buffer
#define BLringbuf_sizeindex_for_ptr(count) BLringbuf_sizeindex(count, sizeof(void*))

#pragma endregion ring_buffer_without_exclusive_control
#pragma region ring_buffer_with_exclusive_control
/*!
\struct BLringbufex
\brief ring buffer with exclusive control
*/
typedef struct {
    pthread_mutex_t mutex;
    BLringbuf_t buf;
} BLringbufex_t, *pBLringbufex_t;
typedef const BLringbufex_t *pcBLringbufex_t;

/*!
\brief create a new ring buffer object with exclusive control
\param count_bits [in] 1..16, effective bit count of mask, wrptr, rdptr.  buffer size
is 2^(count_bits - 1).
\param ppobj [out]
\return errno compatible number
*/
int BLringbufex_new(uint8_t count_bits, pBLringbufex_t* ppobj);

/*!
\brief create a new ring buffer which can hold at the least least_count.
\param least_count [in] minimum required element count.
\param ppobj [out]
\retur errno compatible number
*/
int BLringbufex_newptrqueue(uint16_t least_count, pBLringbufex_t* ppobj);

/*!
\brief delete an existing ring buffer object with exclusive control
\param ppobj [in,out]
*/
void BLringbufex_delete(pBLringbufex_t *ppobj);

/*!
\brief delete an existing ring buffer as a pointer queue.
*/
void BLringbufex_deleteptrqueue(pBLringbufex_t* ppobj);

int BLringbufex_lock(pBLringbufex_t obj);
int BLringbufex_unlock(pBLringbufex_t obj);

/*!
\brief 1) lock, 2) put pv, 3) unlock
\return EXIT_SUCCESS: success, ENOSPC: buffer full and no change is applied to buffer.
*/
int BLringbufex_putptr(pBLringbufex_t obj, void* pv);

/*!
\brief 1) lock, 2) get, 3) unlock, 4) return get result.
\return NULL: no data, valid pointer: success
*/
void* BLringbufex_getptr(pBLringbufex_t obj);
#pragma endregion ring_buffer_with_exclusive_control
#ifdef __cplusplus
}
#endif
#endif /* BLRINGBUF_H_ */