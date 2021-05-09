#ifndef UASDKPKTIO_H_
#define UASDKPKTIO_H_
#include    "sdk/UASDKiobase.h"
#include    "sdk/UASDKpkt.h"
#include    "base/BLbase.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    UASDKpktiocmd_none, // no request
    UASDKpktiocmd_stop, // stop readthread
    UASDKpktiocmd_reset, // reset rxbuf
    UASDKpktiocmd_ready, // readthread() set this value to show being ready to accept a new cmd
} UASDKpktiocmd_t;

typedef void (*UASDKpkt_callback_t)(pcUASDKpkt_t rxoutpkt, void* context);
typedef struct {
    pcUASDKuartdescriptor_t uart;
    UASDKpkt_callback_t rxcallback;
    void* callback_context;
    pthread_t thread;
    UASDKpktiocmd_t cmd; // command executed when SIGINT is sent to readthread()
} UASDKpktio_reader_t, *pUASDKpktio_reader_t;
typedef const UASDKpktio_reader_t *pcUASDKpktio_reader_t;

/*!
\brief allocate resources and start read thread.
\param reader [in,out] packet reader object
*/
int UASDKpktio_start(pUASDKpktio_reader_t reader);

/*!
\brief stop read thread and release resources
\param reader [in,out] packet reader object
*/
int UASDKpktio_stop(pUASDKpktio_reader_t reader);

/*!
\brief reset buffers in the reader
*/
int UASDKpktio_reset(pUASDKpktio_reader_t reader);

/*!
\brief blocking write
\param uart [in] serialport descriptor
\param pkt [in] data to write
\return unix errno compatible result status number
*/
int UASDKpktio_writesync(pcUASDKuartdescriptor_t uart, pcUASDKpkt_t pkt);
#ifdef __cplusplus
}
#endif
#endif /* UASDKPKTIO_H_ */