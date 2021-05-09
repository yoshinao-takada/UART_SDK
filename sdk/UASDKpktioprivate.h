#ifndef UASDKPKTIOPRIVATE_H_
#define UASDKPKTIOPRIVATE_H_
#include    "sdk/UASDKpkt.h"
#include    "base/BLbase.h"
#include    "base/BLringbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    UASDKunibuf_t rxbuf0; // 1st stage rx buffer
    pBLringbuf_t rxbuf1; // 2nd stage rx buffer
    UASDKunibuf_t rxbuf2; // packetized buffer, 3rd stage buffer
    pUASDKpkt_t rxpkt; // alias of rxbuf2
} UASDKpktio_rxbuf_t, *pUASDKpktio_rxbuf_t;


#define RXBUF_INIDEF { \
    UASDKunibuf_initdef, NULL, UASDKunibuf_initdef, NULL \
}

void UASDKpktio_rxbuf_reset(pUASDKpktio_rxbuf_t rxbuf);
#ifdef __cplusplus
}
#endif
#endif /* UASDKPKTIOPRIVATE_H_ */