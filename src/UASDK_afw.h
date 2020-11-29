#ifndef UASDK_AFW_H_
#define UASDK_AFW_H_
#include    <string.h>
#include    <strings.h>
#include    <memory.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <assert.h>
#include    <errno.h>
#include    <malloc.h>
#include    "UASDK_setup.h"
#include    "UASDK_buffer.h"
#include    "UASDK_addressable.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    pUASDK_commport_t commport; // main commport
    pUASDK_addressable_t addressable; // my address and addresses of devices to communicate with
} UASDK_afx_t, *pUASDK_afx_t;
typedef const UASDK_afx_t *pcUASDK_afx_t;

/*!
\brief Tx message, blocking operation
\param afx [in] app framework object
\param your_index [in] index of your address
\param size [in] data size to transmit
\param data [in] data to transmit
\param retry [in] maximum retry
\param waitbytes_for_retry [in] wait time for retry counted in number of bytes on the serialline.
\return errno
*/
int UASDK_afx_tx(pUASDK_afx_t afx, int your_index, uint8_t size, const uint8_t* data,
    int retry, int waitbytes_for_retry);

/*!
\brief Tx message, nonblocking operation with receiving callback
\param afx [in] app framework object
\param your_index [in] index of your address
\param size [in] data size to transmit
\param data [in] data to transmit
\param cb [in] callback method
\param cb_arg [in] callback argument
\return errno
*/
int UASDK_afx_tx_rxcallback(pUASDK_afx_t afx, int your_index, uint8_t size, const uint8_t* data,
    void* (*cb)(void*), void* cb_arg);

/*!
\brief Rx message, non-blocking operation
\param afx [in] app framework object
\param your_index [in] index of your address
\param cb [in] callback method
\param cb_arg [in] callback argument
\return errno
*/
int UASDK_afx_rxcallback(pUASDK_afx_t afx, int your_index, void* (*cb)(void*), void* cb_arg);

#ifdef __cplusplus
}
#endif

#endif /* UASDK_AFW_H_ */