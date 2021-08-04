#include    "CRCTestLib.h"

uint16_t CRCTestRun(const uint16_t* data, const uint16_t* error, uint16_t seed)
{
    BLcrc_t crc;
    BLcrc_init(&crc, CRC_CCITT_LE, seed);
    for (int i = 0; i < DATA_ARRAY_SIZE; i++)
    {
        BLcrc_put16(&crc, data + i);
    }
    uint16_t crc_code = crc.crc.u16[0];
    BLcrc_init(&crc, CRC_CCITT_LE, seed);
    for (int i = 0; i < DATA_ARRAY_SIZE; i++)
    {
        uint16_t error_data = data[i] ^ error[i];
        BLcrc_put16(&crc, &error_data);
    }
    printf("CRC code = 0x%04x\n", crc_code);
    BLcrc_put16(&crc, &crc_code);
    return crc.crc.u16[0];
}


