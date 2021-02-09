#ifndef UASDK_PACKET_H_
#define UASDK_PACKET_H_
#include    "base/BLbase.h"
#include    "base/BLarray1D.h"
#ifdef __cplusplus
extern "C" {
#endif
#pragma region PHYSICAL_LAYER
typedef struct {
    uint8_t rad; // recipient address
    uint8_t sad; // sender address
    uint16_t pid; // packet id
    uint8_t ptp;  // packet type
    uint8_t pll; // payload length
    uint16_t edc; // error detection code
} UASDK_packet_header_t, *pUASDK_packet_header_t;
typedef const UASDK_packet_header_t *pcUASDK_packet_header_t;

typedef struct {
    UASDK_packet_header_t header;
    uint8_t payload[0];
} UASDK_packet_t, *pUASDK_packet_t;
typedef const UASDK_packet_t *pcUASDK_packet_t;

typedef struct {
    uint16_t payload_buffer_byte_count;
    UASDK_packet_t packet;
} UASDK_packet_buffer_t, *pUASDK_packet_buffer_t;
typedef const UASDK_packet_buffer_t *pcUASDK_packet_buffer_t;

#define UASDK_header(packet_buffer)     (pUASDK_packet_header_t)&((packet_buffer)->packet.header)
#define UASDK_const_header(packet_buffer)     (pcUASDK_packet_header_t)&((packet_buffer)->packet.header)
#define UASDK_payload(packet_buffer)    (uint8_t*)((packet_buffer)->packet.payload)
#define UASDK_const_payload(packet_buffer) (const uint8_t*)((packet_buffer)->packet.payload)

/*!
\brief create an object of a packet buffer
\param payload_byte_count [in] payload buffer byte count
\param packet_buffer [out]
\return unix errno compatible error code
*/
int UASDK_packet_buffer_new(uint16_t payload_byte_count, pUASDK_packet_buffer_t* packet_buffer);
#pragma endregion PHYSICAL_LAYER

#pragma region DATA_LINK_AND_NETWORK_LAYER
/*!
\brief check edc
\param header [in] packet header
\return EXIT_SUCCESS: edc is valid, EXIT_FAILURE: edc is invalid
*/
int UASDK_packet_header_check_edc(pcUASDK_packet_header_t header);

/*!
\brief set edc
\param header [in,out] packet header
*/
void UASDK_packet_header_set_edc(pUASDK_packet_header_t header);

/*!
\brief check edc
\param packet [in]
\return EXIT_SUCCESS: edc is valid, EXIT_FAILURE: edc is invalid
*/
int UASDK_packet_check_edc(pcUASDK_packet_t packet);

/*!
\brief set edc
\param packet [in]
*/
int UASDK_packet_set_edc(pUASDK_packet_t packet);
#pragma endregion DATA_LINK_AND_NETWORK_LAYER
#pragma region  TRANSPORT_LAYER
// result code of each (transmission-reception) operation
typedef enum {
    RESCODE_SUCCESS = 0,
    RESCODE_WARN = 1,
    RESCODE_COMM_ERR = 2,
    RESCODE_BUSY_ERR = 3,
    RESCODE_OPNSUP_ERR = 4,
    RESCODE_INVPAR_ERR = 5,
    RESCODE_DEVPROBLEM_ERR = 6,
    RESCODE_END
} UASDK_rescode_t;


#pragma endregion  TRANSPORT_LAYER
#ifdef __cplusplus
}
#endif
#endif /* UASDK_PACKET_H_ */