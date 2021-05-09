#include    "sdk/UASDKpktio.h"
#include    "sdk/UASDKpktioprivate.h"
#include    "sdk/UASDKpktiostate.h"
#include    "sdk/UASDKpkt_testgen.h"
#include    <string.h>
static void test_callback(pcUASDKpkt_t rxoutpkt, void* context);
typedef struct {
    pcUASDKpkt_t pktref;
    int compare_result; // EXIT_SUCCESS: pktref == rxoutpkt, EXIT_FAILURE: pktref != rxoutpkt
} pktio_context_t, *ppktio_context_t;
typedef const pktio_context_t *pcpktio_context_t;
static pktio_context_t g_context = { NULL, EXIT_SUCCESS };
static UASDKpktio_rxbuf_t rxbuf = RXBUF_INIDEF;
static UASDKpktio_reader_t reader = { NULL, test_callback, (void*)&g_context, 0, UASDKpktiocmd_ready };
static UASDKpkt_testgen_t testgen, testgen2;
static const UASDKpktio_state_t states[] = STATES_DEF;
#pragma region test_support_functions
static void test_callback(pcUASDKpkt_t rxoutpkt, void* context_)
{
    fprintf(stderr, "%s enter\n", __FUNCTION__);
    ppktio_context_t context = (ppktio_context_t)context_;
    if (EXIT_SUCCESS != (context->compare_result = UASDKpkt_checkedc(rxoutpkt)))
    {
        fprintf(stderr, "%s: CRC error\n", __FUNCTION__);
        return;
    }
    int pktref_size = UASDKpkt_totalbytes(context->pktref);
    int pkt_rx_size = UASDKpkt_totalbytes(rxoutpkt);
    if (pktref_size != pkt_rx_size) 
    {
        context->compare_result = EXIT_FAILURE;
        fprintf(stderr, "%s: size mismatch (ref_size, rx_size) = (%d, %d)\n",
            __FUNCTION__, pktref_size, pkt_rx_size);
        return;
    }
    if (memcmp(context->pktref, rxoutpkt, pktref_size))
    {
        context->compare_result = EXIT_FAILURE;
        fprintf(stderr, "%s: (ref_header, rx_header) = (0x%02x, 0x%02x, 0x%04x: 0x%02x, 0x%02x, 0x%04x)\n",
            __FUNCTION__, context->pktref->rad, context->pktref->sad, context->pktref->opt.pll,
            rxoutpkt->rad, rxoutpkt->sad, rxoutpkt->opt.pll);
        return;
    }
    context->compare_result = EXIT_SUCCESS;
    UT_SHOW(stderr, __FUNCTION__, __LINE__, context->compare_result);

}

static int init_rxbuf()
{
    int err = EXIT_SUCCESS;
    do {
        if ((EXIT_SUCCESS != (err = UASDKunibuf_initbyte(&rxbuf.rxbuf0, 256))) ||
            (EXIT_SUCCESS != (err = UASDKunibuf_initbyte(&rxbuf.rxbuf2, 256))) ||
            (EXIT_SUCCESS != (err = BLringbuf_new(10, &rxbuf.rxbuf1))))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        rxbuf.rxpkt = rxbuf.rxbuf2.voidbuf.buf;
        UASDKpkt_testgen_init(&testgen);
        UASDKpkt_testgen_init(&testgen2);
        g_context.pktref = testgen.pkt;
    } while (0);
    return err;
}

static int destroy_rxbuf()
{
    UASDKunibuf_destroy(&rxbuf.rxbuf0);
    UASDKunibuf_destroy(&rxbuf.rxbuf2);
    BLSAFEFREE(&rxbuf.rxbuf1);
    rxbuf.rxpkt = NULL;
    UASDKpkt_testgen_destroy(&testgen);
    return EXIT_SUCCESS;
}
#pragma endregion test_support_functions

// test transition from empty-continue to empty-nodata with payloadless packet
int pktiostate_empty_nodata()
{
    static const UASDKpkt_gensrc_t pkt_gen_src[] = {
        { UASDKpkt_gentype_short, 0x41, 0x81, { (uint16_t)0x5678 }, { NULL } },
        { UASDKpkt_gentype_short, 0x42, 0x91, { (uint16_t)0x5678 }, { NULL } }
    };
    uint16_t actually_copied, byte_count_to_copy;
    int err = EXIT_SUCCESS;
    State_t state = { StateID_empty, Substate_continue };
    do {
        State_t next_state = states[state.state].state_handler(&rxbuf, &reader);
        if (next_state.state != StateID_empty || next_state.substate != Substate_nodate)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        UASDKpkt_testgen(&testgen, &pkt_gen_src[0]);
        byte_count_to_copy = (uint16_t)UASDKpkt_totalbytes(testgen.pkt);
        if (EXIT_SUCCESS != (err = BLringbuf_put(rxbuf.rxbuf1, byte_count_to_copy, (const void*)testgen.pkt, &actually_copied)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        UASDKpkt_testgen(&testgen, &pkt_gen_src[1]);
        if (EXIT_SUCCESS != (err = BLringbuf_put(rxbuf.rxbuf1, byte_count_to_copy, (const void*)testgen.pkt, &actually_copied)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        next_state = states[state.state].state_handler(&rxbuf, &reader);
        if (next_state.state != StateID_empty || next_state.substate != Substate_continue)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        state = next_state;
        next_state = states[state.state].state_handler(&rxbuf, &reader);
        if (next_state.state != StateID_empty || next_state.substate != Substate_continue)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        state = next_state;
        next_state = states[state.state].state_handler(&rxbuf, &reader);
        if (next_state.state != StateID_empty || next_state.substate != Substate_nodate)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err; 
}

int pktiostate_empty_incomplete()
{
    static const UASDKpkt_gensrc_t pkt_gen_src[] = {
        { UASDKpkt_gentype_random, 0x41, 0x81, { (uint16_t)16 }, { NULL } },
        { UASDKpkt_gentype_random, 0x42, 0x91, { (uint16_t)64 }, { NULL } },
        { UASDKpkt_gentype_short, 0x43, 0xa1, { (uint16_t)0 }, { NULL } }
    };
    uint16_t actually_copied, byte_count_to_copy;
    int err = EXIT_SUCCESS;
    State_t state = { StateID_empty, Substate_continue };
    UASDKpktio_rxbuf_reset(&rxbuf);
    do {
        UASDKpkt_testgen(&testgen, &pkt_gen_src[0]);
        byte_count_to_copy = UASDKpkt_totalbytes(testgen.pkt);
        if (EXIT_SUCCESS != (err = BLringbuf_put(rxbuf.rxbuf1, byte_count_to_copy, (const void*)testgen.pkt, &actually_copied)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        State_t next_state = states[state.state].state_handler(&rxbuf, &reader);
        if (next_state.state != StateID_payload_incomplete || next_state.substate != Substate_continue)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        state = next_state;
        next_state = states[state.state].state_handler(&rxbuf, &reader);
        if ((next_state.state != StateID_empty) || (next_state.substate != Substate_continue))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        fprintf(stderr, "1st packet complete\n");
        state = next_state;
        UASDKpkt_testgen(&testgen, &pkt_gen_src[1]);
        byte_count_to_copy = UASDKpkt_totalbytes(testgen.pkt);
        byte_count_to_copy -= 8;
        if (EXIT_SUCCESS != (err = BLringbuf_put(rxbuf.rxbuf1, byte_count_to_copy, (const void*)testgen.pkt, &actually_copied)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        // get 2nd packet header
        next_state = states[state.state].state_handler(&rxbuf, &reader);
        if ((next_state.state != StateID_payload_incomplete) || (next_state.substate != Substate_continue))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        state = next_state;
        // get 2nd packet payload available part
        next_state = states[state.state].state_handler(&rxbuf, &reader);
        // available data is insufficient and next_state.substate must be Substate_nodata.
        if ((next_state.state != StateID_payload_incomplete) || (next_state.substate != Substate_nodate))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        // put remaining part of the payload and the next packet
        UASDKpkt_testgen(&testgen2, &pkt_gen_src[2]);        
        state = next_state;
        if (EXIT_SUCCESS != (err = BLringbuf_put(rxbuf.rxbuf1, 8, testgen.buf.bytebuf.buf + actually_copied, &actually_copied)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        byte_count_to_copy = UASDKpkt_totalbytes(testgen2.pkt);
        if (EXIT_SUCCESS != (err = BLringbuf_put(rxbuf.rxbuf1, byte_count_to_copy, (const void*)testgen2.pkt, &actually_copied)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        // get the remaining part of the payload of the 2nd packet
        next_state = states[state.state].state_handler(&rxbuf, &reader);
        if ((next_state.state != StateID_empty) || (next_state.substate != Substate_continue))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
        // get the 3rd packet
        g_context.pktref = testgen2.pkt;
        state = next_state;
        next_state = states[state.state].state_handler(&rxbuf, &reader);
        if ((next_state.state != StateID_empty) || (next_state.substate != Substate_continue))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err; 
}

int pktiostate()
{
    int err = EXIT_SUCCESS, err_each = EXIT_SUCCESS;
    do {
        err |= (err_each = init_rxbuf());
        if (err_each)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err_each);
        }
        err |= (err_each = pktiostate_empty_nodata());
        if (err_each)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err_each);
        }
        err |= (err_each = pktiostate_empty_incomplete());
        if (err_each)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err_each);
        }
        err |= (err_each = destroy_rxbuf());
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err_each);
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}