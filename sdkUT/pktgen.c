#include    "sdk/UASDKpkt_testgen.h"
#include    "base/BLbase.h"

static int gentype_data();
static int gentype_constbyte();
static int gentype_random();
static int gentype_short();

int pktgen()
{
    int err = EXIT_SUCCESS;
    int erreach = EXIT_SUCCESS;
    do {
        err |= erreach = gentype_data();
        if (erreach)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        err |= erreach = gentype_constbyte();
        if (erreach)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        err |= erreach = gentype_random();
        if (erreach)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        err |= erreach = gentype_short();
        if (erreach)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static int gentype_data()
{
    static const char srcdata[] = "0123456789ABCDEFG";
    int err = EXIT_SUCCESS;
    do {
        UASDKpkt_testgen_t pktgen = UASDKPKT_TESTGEN_NULL;
        UASDKpkt_gensrc_t gensrc = { UASDKpkt_gentype_data, 0x40, 0x81, {16}, {(const void*)srcdata}};
        UASDKpkt_testgen_init(&pktgen);
        UASDKpkt_testgen(&pktgen, &gensrc);
        if (EXIT_SUCCESS != (err = UASDKpkt_checkedc(pktgen.pkt)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if ((0x40 != pktgen.pkt->rad) || (0x81 != pktgen.pkt->sad) || (16 != pktgen.pkt->opt.pll))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        for (uint16_t i = 0; i < pktgen.pkt->opt.pll; i++)
        {
            if (srcdata[i] != pktgen.pkt->payload[i])
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
            }
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static int gentype_constbyte()
{
    int err = EXIT_SUCCESS;
    do {
        UASDKpkt_testgen_t pktgen = UASDKPKT_TESTGEN_NULL;
        UASDKpkt_gensrc_t gensrc = { UASDKpkt_gentype_constbyte, 0x40, 0x81, {16}, {(const void*)(uintptr_t)0x56}};
        UASDKpkt_testgen_init(&pktgen);
        UASDKpkt_testgen(&pktgen, &gensrc);
        if (EXIT_SUCCESS != (err = UASDKpkt_checkedc(pktgen.pkt)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if ((0x40 != pktgen.pkt->rad) || (0x81 != pktgen.pkt->sad) || (16 != pktgen.pkt->opt.pll))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        uint8_t constbyte = gensrc.payload_src.const_byte;
        for (uint16_t i = 0; i < pktgen.pkt->opt.pll; i++)
        {
            if (constbyte != pktgen.pkt->payload[i])
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
            }
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static int gentype_random()
{
    int err = EXIT_SUCCESS;
    do {
        UASDKpkt_testgen_t pktgen = UASDKPKT_TESTGEN_NULL;
        srand(1);
        UASDKpkt_gensrc_t gensrc = { UASDKpkt_gentype_random, 0x40, 0x81, {16}, { NULL }};
        UASDKpkt_testgen_init(&pktgen);
        UASDKpkt_testgen(&pktgen, &gensrc);
        if (EXIT_SUCCESS != (err = UASDKpkt_checkedc(pktgen.pkt)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if ((0x40 != pktgen.pkt->rad) || (0x81 != pktgen.pkt->sad) || (16 != pktgen.pkt->opt.pll))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        srand(1);
        for (uint16_t i = 0; i < pktgen.pkt->opt.pll; i++)
        {
            uint8_t refbyte = (uint8_t)(0xff & rand());
            if (refbyte != pktgen.pkt->payload[i])
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
            }
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static int gentype_short()
{
    int err = EXIT_SUCCESS;
    do {
        UASDKpkt_testgen_t pktgen = UASDKPKT_TESTGEN_NULL;
        UASDKpkt_gensrc_t gensrc = { UASDKpkt_gentype_short, 0x40, 0x81, { 0x8878 }, { NULL }};
        UASDKpkt_testgen_init(&pktgen);
        UASDKpkt_testgen(&pktgen, &gensrc);
        if (EXIT_SUCCESS != (err = UASDKpkt_checkedc(pktgen.pkt)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if ((0x40 != pktgen.pkt->rad) || (0x81 != pktgen.pkt->sad) || (gensrc.opt.short_data != pktgen.pkt->opt.pll_sign))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}
