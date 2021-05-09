#ifndef UASDKPKT_TESTGEN_H_
#define UASDKPKT_TESTGEN_H_
#include    "sdk/UASDKpkt.h"
#include    "sdk/UASDKiobase.h"
#ifdef __cplusplus
extern "C" {
#endif
#pragma region UASDKpkt_testgen_component_interface
#define DEFBUFSIZE  256
typedef struct {
    UASDKunibuf_t buf;
    pUASDKpkt_t pkt; // alias of buf.bytebuf.buf
} UASDKpkt_testgen_t, *pUASDKpkt_testgen_t;
typedef const UASDKpkt_testgen_t *pcUASDKpkt_testgen_t;
#define UASDKPKT_TESTGEN_NULL   { UASDKunibuf_initdef, NULL }

/*!
\brief allocate memory block 256 bytes
*/
void UASDKpkt_testgen_init(pUASDKpkt_testgen_t pkt);

/*!
\brief free allocated memory block
*/
void UASDKpkt_testgen_destroy(pUASDKpkt_testgen_t pkt);

/*!
\brief set addresses
*/
void UASDKpkt_testgen_setaddresses(pUASDKpkt_testgen_t pkt, uint8_t rad, uint8_t sad);

/*!
\brief set payload data and calculate CRC. Abort if payload_size > 250
*/
void UASDKpkt_testgen_setpayload(pUASDKpkt_testgen_t pkt, uint16_t payload_size, const void* payload_data);

/*!
\brief set payload data and caculate CRC. Abort if payload_size > 250. payload data is a const value.
*/
void UASDKpkt_testgen_setpayload_byteseq(pUASDKpkt_testgen_t pkt, uint16_t payload_size, uint8_t const_byte);

/*!
\brief set payload data and caculate CRC. Abort if payload_size > 250. payload data is random numbers.
*/
void UASDKpkt_testgen_setpayload_random(pUASDKpkt_testgen_t pkt, uint16_t payload_size);

/*!
\brief set short data to create a payloadless packet
*/
void UASDKpkt_testgen_setshortdata(pUASDKpkt_testgen_t pkt, int16_t short_data);
#pragma endregion UASDKpkt_testgen_component_interface
#pragma region UASDKpkt_testgen_integrated_interface
typedef enum {
    UASDKpkt_gentype_data,
    UASDKpkt_gentype_constbyte,
    UASDKpkt_gentype_random,
    UASDKpkt_gentype_short
} UASDKpkt_gentype_t;

typedef struct {
    UASDKpkt_gentype_t gentype;
    uint8_t rad;
    uint8_t sad;
    union {
        uint16_t payload_size;
        int16_t short_data;
    } opt;
    union {
        const void* payload_data;
        uint8_t const_byte;
    } payload_src;
} UASDKpkt_gensrc_t, *pUASDKpkt_gensrc_t;
typedef const UASDKpkt_gensrc_t *pcUASDKpkt_gensrc_t;

void UASDKpkt_testgen(pUASDKpkt_testgen_t pkt, pcUASDKpkt_gensrc_t gensrc);
#pragma endregion UASDKpkt_testgen_integrated_interface
#ifdef __cplusplus
}
#endif
#endif /* UASDKPKT_TESTGEN_H_ */