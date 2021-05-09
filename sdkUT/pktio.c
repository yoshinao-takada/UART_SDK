#include    "sdk/UASDKpktio.h"
#include    "sdk/UASDKpkt_testgen.h"
#include    <unistd.h>
#include    <string.h>
#define DEVNAME0    "/dev/ttyUSB0"
#define DEVNAME1    "/dev/ttyUSB1"
static const UASDKbaudrate_t baud = UASDKBAUD(9600);
static UASDKuartdescriptor_t uartTx = UASDKUARTDEF0, uartRx = UASDKUARTDEF0;
static UASDKpkt_testgen_t pktsrc = { UASDKunibuf_initdef, NULL };
typedef struct {
    pcUASDKpkt_t pktref;
    int result;
} pkt_reader_context_t, *ppkt_reader_context_t;
typedef const pkt_reader_context_t *pcpkt_reader_context_t;

static void pkt_reader_callback(pcUASDKpkt_t rxoutpkt, void* context_)
{
    fprintf(stderr, "%s enter\n", __FUNCTION__);
    ppkt_reader_context_t context = (ppkt_reader_context_t)context_;
    int pktref_size = UASDKpkt_totalbytes(context->pktref);
    int pkt_rx_size = UASDKpkt_totalbytes(rxoutpkt);
    if (pktref_size != pkt_rx_size) 
    {
        context->result = EXIT_FAILURE;
        fprintf(stderr, "%s: size mismatch (ref_size, rx_size) = (%d, %d)\n",
            __FUNCTION__, pktref_size, pkt_rx_size);
        return;
    }
    if (memcmp(context->pktref, rxoutpkt, pktref_size))
    {
        context->result = EXIT_FAILURE;
        fprintf(stderr, "%s: (ref_header, rx_header) = (0x%02x, 0x%02x, 0x%04x: 0x%02x, 0x%02x, 0x%04x)\n",
            __FUNCTION__, context->pktref->rad, context->pktref->sad, context->pktref->opt.pll,
            rxoutpkt->rad, rxoutpkt->sad, rxoutpkt->opt.pll);
        return;
    }
    context->result = EXIT_SUCCESS;
    UT_SHOW(stderr, __FUNCTION__, __LINE__, context->result);
}

int pktio();

int open_uart()
{
    int err = EXIT_SUCCESS;
    do {
        if ((EXIT_SUCCESS != (err = UASDKiobase_open(&uartTx, DEVNAME0, &baud, UASDKbyteformat_N1))) ||
            (EXIT_SUCCESS != (err = UASDKiobase_open(&uartRx, DEVNAME1, &baud, UASDKbyteformat_N1))))
        {
            break;
        }
        UASDKpkt_testgen_init(&pktsrc);
    } while (0);
    return err;
}

int close_uart()
{
    int err = EXIT_SUCCESS;
    do {
        if (close(uartTx.fd))
        {
            err = errno;
            break;
        }
        if (close(uartRx.fd))
        {
            err = errno;
            break;
        }
        UASDKpkt_testgen_destroy(&pktsrc);
    } while (0);
    return err;
}

/*!
\brief synchronized write of random packet
*/
int writesync(uint8_t rad, uint8_t sad, uint16_t payload_size)
{
    int err = EXIT_SUCCESS;
    const UASDKpkt_gensrc_t gensrc = { UASDKpkt_gentype_random, rad, sad, {payload_size}, { NULL }};
    do {
        if (payload_size > 200)
        {
            err = ERANGE;
            break;
        }
        UASDKpkt_testgen(&pktsrc, &gensrc);
        if (EXIT_SUCCESS != (err = UASDKpktio_writesync(&uartTx, pktsrc.pkt)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
    } while (0);
    return err;
}

typedef struct {
    uint8_t rad;
    uint8_t sad;
    uint16_t payload_size;
} pkt_header_src_t, *ppkt_header_src_t;
typedef const pkt_header_src_t *pcpkt_header_src_t;

static const pkt_header_src_t header_src[] = {
    { 0x84, 0x44, 0x0064 },
    { 0x94, 0x45, 0x0032 },
    { 0xa4, 0x46, 0x0048 },
    { 0xb4, 0x47, 0x0016 }
};

int pktio()
{
    int err = EXIT_SUCCESS;
    UASDKuartdescriptor_t uart = UASDKUARTDEF0;
    pkt_reader_context_t context = { NULL, 0 };
    UASDKpktio_reader_t reader = { &uartRx, pkt_reader_callback, &context, 0, UASDKpktiocmd_none };
    do {
        if (EXIT_SUCCESS != (err = open_uart()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        context.pktref = pktsrc.pkt;
        if (EXIT_SUCCESS != (err = UASDKpktio_start(&reader)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        for (int i = 0; i < ARRAYSIZE(header_src); i++)
        {
            sleep(1);
            pcpkt_header_src_t p = &header_src[i];
            if (EXIT_SUCCESS != (err = writesync(p->rad, p->sad, p->payload_size)))
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
            }           
        }
        fprintf(stderr, "%s,%d,calling UASDKpktio_stop()\n", __FUNCTION__, __LINE__);
        if (EXIT_SUCCESS != (err = UASDKpktio_stop(&reader)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        if (EXIT_SUCCESS != (err = close_uart()))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}