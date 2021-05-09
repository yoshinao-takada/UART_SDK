#include    "sdk/UASDKpkt.h"
#include    "base/BLcrc.h"
#include    <stdlib.h>
#include    <memory.h>
#include    <assert.h>
#include    <stdio.h>
#include    <errno.h>

int UASDKpkt_new(pUASDKpkt_t* pppkt, uint16_t payload_byte_count)
{
    int err = EXIT_SUCCESS;
    pUASDKpkt_t p = NULL;
    assert(payload_byte_count < 0x8000);
    do {
        size_t cb_alloc = sizeof(UASDKpkt_t) + payload_byte_count;
        *pppkt = (p = (pUASDKpkt_t)malloc(cb_alloc));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        memset(p, 0, cb_alloc);
        p->opt.pll = (payload_byte_count == 0) ? 0x8000 : payload_byte_count;
    } while (0);
    return err;
}

void UASDKpkt_setedc(pUASDKpkt_t pkt)
{
    uint32_t crc_source_bytes = UASDKpkt_totalbytes(pkt) - 2;
    BLcrc_t crc;
    BLcrc_init(&crc, CRC_CCITT_LE, 0);
    uint8_t *pkt_begin = (uint8_t*)pkt;
    BLcrc_puts(&crc, pkt_begin, crc_source_bytes);
    uint16_t* crc_code = (uint16_t*)(pkt_begin + crc_source_bytes);
    *crc_code = crc.crc.u16[0];
}

int UASDKpkt_checkedc(pcUASDKpkt_t pkt)
{
    uint32_t crc_source_bytes = UASDKpkt_totalbytes(pkt);
    BLcrc_t crc;
    BLcrc_init(&crc, CRC_CCITT_LE, 0);
    uint8_t *pkt_begin = (uint8_t*)pkt;
    BLcrc_puts(&crc, pkt_begin, crc_source_bytes);
    return crc.crc.u16[0] ? EINVAL : EXIT_SUCCESS;
}

int UASDKpkt_setasd1(pUASDKpkt_t pkt, uint8_t asd1)
{
    int err = EXIT_SUCCESS;
    do {
        if (pkt->opt.pll_sign > 0)
        {
            err = EINVAL;
            break;
        }
        pkt->opt.asd[1] = (0x80 | asd1);
    } while (0);
    return err;
}

int UASDKpkt_getasd1(pcUASDKpkt_t pkt, uint8_t* asd1)
{
    int err = EXIT_SUCCESS;
    do {
        if (pkt->opt.pll_sign > 0)
        {
            err = EINVAL;
            break;
        }
        *asd1 = 0x7f & (pkt->opt.asd[1]);
    } while (0);
    return err;
}

bool UASDKpkt_isfullyfilled(pcUASDKpkt_t pkt, int filled_bytes)
{
    if (filled_bytes < sizeof(UASDKpkt_t)) return false;
    if (pkt->opt.pll_sign < 0) return true;
    int total_bytes = UASDKpkt_totalbytes(pkt);
    return (total_bytes <= filled_bytes);
}