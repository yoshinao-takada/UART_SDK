#include    "sdk/UASDKpkt.h"
#include    "base/BLbase.h"
#include    <memory.h>

int pkt();

static int pkt_new();
static int pkt_edc();
static int pkt_asd1();

int pkt()
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = pkt_new()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (EXIT_SUCCESS != (err = pkt_edc()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (EXIT_SUCCESS != (err = pkt_asd1()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static int pkt_new()
{
    int err = EXIT_SUCCESS;
    pUASDKpkt_t pkt = NULL;
    do {
        if (EXIT_SUCCESS != (err = UASDKpkt_new(&pkt, 0)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (pkt->opt.pll != 0x8000)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        BLSAFEFREE(&pkt);
        const uint16_t payload_byte_count = 88;
        if (EXIT_SUCCESS != (err = UASDKpkt_new(&pkt, payload_byte_count)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (pkt->opt.pll != payload_byte_count)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        if (UASDKpkt_totalbytes(pkt) != (payload_byte_count + 6))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
    } while (0);
    BLSAFEFREE(&pkt);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static int pkt_edc()
{
    int err = EXIT_SUCCESS;
    pUASDKpkt_t pkt = NULL;
    do {
        if (EXIT_SUCCESS != (err = UASDKpkt_new(&pkt, 0)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        pkt->rad = 0x80;
        pkt->sad = 0x81;
        if (EXIT_SUCCESS != (err = UASDKpkt_setasd1(pkt, 0x55)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (0xd500 != pkt->opt.pll)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        pkt->payload[0] = pkt->payload[1] = 0;
        UASDKpkt_setedc(pkt);
        if (EXIT_SUCCESS != (err = UASDKpkt_checkedc(pkt)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        pkt->rad ^= 0x40;
        if (EXIT_SUCCESS == UASDKpkt_checkedc(pkt))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err == EXIT_FAILURE));
        }
        BLSAFEFREE(&pkt);
        const uint16_t payload_byte_count = 100;
        if (EXIT_SUCCESS != (err = UASDKpkt_new(&pkt, payload_byte_count)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        srand(0);
        uint16_t i = 0;
        for (; i < payload_byte_count; i++)
        {
            pkt->payload[i] = (uint8_t)rand();
        }
        pkt->payload[i] = 0; pkt->payload[i + 1] = 0;
        UASDKpkt_setedc(pkt);
        if (EXIT_SUCCESS != (err = UASDKpkt_checkedc(pkt)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        pkt->payload[i] ^= 0x01;
        if (EXIT_SUCCESS == UASDKpkt_checkedc(pkt))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
    } while (0);
    BLSAFEFREE(&pkt);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static int pkt_asd1()
{
    pUASDKpkt_t pkt = NULL;
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = UASDKpkt_new(&pkt, 0)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        pkt->rad = 0x80;
        pkt->sad = 0x81;
        if (EXIT_SUCCESS != (err = UASDKpkt_setasd1(pkt, 0x55)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (0xd500 != pkt->opt.pll)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        uint8_t asd1 = 0;
        if ((EXIT_SUCCESS != (err = UASDKpkt_getasd1(pkt, &asd1))) ||
            (asd1 != 0x55))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        BLSAFEFREE(&pkt);
        const uint16_t payload_byte_count = 100;
        if (EXIT_SUCCESS != (err = UASDKpkt_new(&pkt, payload_byte_count)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        pkt->rad = 0x80;
        pkt->sad = 0x81;
        if (EINVAL != UASDKpkt_setasd1(pkt, 0x55))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        asd1 = 0;
        if (EINVAL != UASDKpkt_getasd1(pkt, &asd1))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
    } while (0);
    BLSAFEFREE(&pkt);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}
