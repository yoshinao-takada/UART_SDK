#ifndef UASDKPKT_H_
#define UASDKPKT_H_
#ifdef __cplusplus
#include    <cstdint>
#include    <cstdbool>
extern "C" {
#else
#include    <stdint.h>
#include    <stdbool.h>
#endif
typedef struct {
    uint8_t rad;
    uint8_t sad;
    union {
        // if (pll <= 0x7fff) it represents a payload length. otherwise asd[] are application specific data
        uint8_t asd[2]; // application specific data
        uint16_t pll; // payload length
        int16_t pll_sign; // it is for discriminating payloadless or not
    } opt; // optional data
    uint8_t payload[2]; // payload[] length is at the least 2. In this case, payload[] contents are only CRC.
                    // otherwise, payload[] contents are data and CRC.
} UASDKpkt_t, *pUASDKpkt_t;
typedef const UASDKpkt_t *pcUASDKpkt_t;

/*!
\brief create a new packet object
\param pppkt [out] new object
\param payload_byte_count [in] payload size excluding CRC (payload accompanying packet).
    if (payload_byte_count == 0) the packet is initialized as a payloadless packet.
\return ESUCCESS or ENOMEM
*/
int UASDKpkt_new(pUASDKpkt_t* pppkt, uint16_t payload_byte_count);

/*!
\brief set error detecting code (CRC-16)
\param pkt [in,out] packet to set an error detecting code.
*/
void UASDKpkt_setedc(pUASDKpkt_t pkt);

/*!
\brief check error detecting code
\param pkt [in] packet to check
\return EXIT_SUCCESS: no error, EINVAL: CRC error
*/
int UASDKpkt_checkedc(pcUASDKpkt_t pkt);

/*!
\brief get the total byte count of the packet
\param ptr_pkt [in]
\return total byte count of the packet.
*/
#define UASDKpkt_totalbytes(pkt) (((pkt)->opt.pll_sign < 0) ? sizeof(UASDKpkt_t) : sizeof(UASDKpkt_t) + (pkt)->opt.pll)

/*!
\brief set the lower 7 bits of pkt->opt.asd[1] if the packet has no payload.
\param pkt [in,out] packet
\param asd1 [in] application specific data
\return EXIT_SUCCESS: valid condition, EINVAL: invaliid condition, the packet is not modified.
*/
int UASDKpkt_setasd1(pUASDKpkt_t pkt, uint8_t asd1);

/*!
\brief get the lower 7 bits of pkt->opt.asd[1] if the packet has no playload.
\param pkt [in] packet
\param asd1 [out] application specific data
\return EXIT_SUCCESS: packet is a valid payloadless packet, EINVAL: packet has a payload.
*/
int UASDKpkt_getasd1(pcUASDKpkt_t pkt, uint8_t* asd1);

/*!
\brief validate packet data
\param pkt [in] packet buffer
\param filled_bytes [in] effective byte count in pkt.
\return true if packet total bytes <= filled_bytes.
*/
bool UASDKpkt_isfullyfilled(pcUASDKpkt_t pkt, int filled_bytes);

#ifdef __cplusplus
}
#endif
#endif /* UASDKPKT_H_ */