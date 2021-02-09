#include    "UASDK_packet.h"
#include    <errno.h>
#pragma region PHYSICAL_LAYER
int UASDK_packet_buffer_new(uint16_t payload_byte_count, pUASDK_packet_buffer_t* packet_buffer)
{
    int err = EXIT_SUCCESS;
    do {
        size_t cb_head = sizeof(UASDK_packet_buffer_t);
        size_t cb_alloc = cb_head + payload_byte_count;
        *packet_buffer = (pUASDK_packet_buffer_t)malloc(cb_alloc);
        if (NULL == *packet_buffer)
        {
            err = ENOMEM;
            break;
        }
        (*packet_buffer)->payload_buffer_byte_count = payload_byte_count;
    } while(0);
    return err;
}
#pragma endregion PHYSICAL_LAYER
#pragma region DATA_LINK_AND_NETWORK_LAYER
static int check_edc(uint32_t buffer_length, const uint8_t* buffer)
{
    uint16_t sum = 0;
    buffer_length -= 2; // exclude edc from the byte count
    for (uint32_t i = 0; i < buffer_length; i++)
    {
        sum += *buffer++;
    }
    sum += *(uint16_t*)buffer;
    return sum == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void set_edc(uint32_t buffer_length, uint8_t* buffer)
{
    uint16_t sum = 0;
    buffer_length -= 2; // exclude edc from the byte count
    for (uint32_t i = 0; i < buffer_length; i++)
    {
        sum += *buffer++;
    }
    *(uint16_t*)buffer = (uint16_t)(0x10000u - sum);
}

int UASDK_packet_header_check_edc(pcUASDK_packet_header_t header)
{
    return check_edc(sizeof(UASDK_packet_header_t), (const uint8_t*)header);
}

void UASDK_packet_header_set_edc(pUASDK_packet_header_t header)
{
    set_edc(sizeof(UASDK_packet_header_t), (uint8_t*)header);
}

int UASDK_packet_check_edc(pcUASDK_packet_t packet)
{
    int check_header = UASDK_packet_header_check_edc(&packet->header);
    if (check_header) return check_header;
    return check_edc((uint32_t)packet->header.pll, packet->payload);
}

int UASDK_packet_set_edc(pUASDK_packet_t packet)
{
    UASDK_packet_header_set_edc(&packet->header);
    set_edc((uint32_t)packet->header.pll, packet->payload);
}
#pragma endregion DATA_LINK_AND_NETWORK_LAYER
#pragma region TRANSPORT_LAYER

#pragma endregion TRANSPORT_LAYER