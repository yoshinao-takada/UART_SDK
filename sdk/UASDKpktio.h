#ifndef UASDKPKTIO_H_
#define UASDKPKTIO_H_
#include    "sdk/UASDKiobase.h"
#include    "sdk/UASDKpkt.h"
#include    "base/BLbase.h"
#include    "base/BLringbuf.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*UASDKpkt_callback_t)(pcUASDKpkt_t rxoutpkt, void* context);

typedef struct {
    UASDKunibuf_t rxbuf0;
    pBLringbuf_t rxbuf1;
    UASDKunibuf_t rxbuf2;
    pUASDKpkt_t rxpkt; // alias of rxbuf2
} UASDKpktio_rxbuf_t, *pUASDKpktio_rxbuf_t;

void UASDKpktio_rxbuf_reset(pUASDKpktio_rxbuf_t rxbuf);

typedef struct {
    UASDKuartdescriptor_t uart;
    UASDKpkt_callback_t rxcallback;
    void* callback_context;
    pthread_t thread;
} UASDKpktio_reader_t, *pUASDKpktio_reader_t;
typedef const UASDKpktio_reader_t *pcUASDKpktio_reader_t;

/*!
\brief allocate resources and start read thread.
\param reader [in,out] packet reader object
\param uart [in] UART descriptor
*/
int UASDKpktio_start(pUASDKpktio_reader_t reader, pcUASDKuartdescriptor_t uart);

/*!
\brief stop read thread and release resources
\param reader [in,out] packet reader object
*/
int UASDKpktio_stop(pUASDKpktio_reader_t reader);

/*!
\brief reset buffers in the reader
*/
void UASDKpktio_reset(pUASDKpktio_reader_t reader);

int UASDKpktio_writesync(pcUASDKuartdescriptor_t uart, pcUASDKpkt_t pkt);
#ifdef __cplusplus
}
#endif
#endif /* UASDKPKTIO_H_ */