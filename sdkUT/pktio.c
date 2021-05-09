#include    "sdk/UASDKpktio.h"
#include    "sdk/UASDKpkt_testgen.h"
#include    "base/BLfile.h"
#include    <unistd.h>
#include    <string.h>
#define DEVNAME0    "/dev/ttyUSB0"
#define DEVNAME1    "/dev/ttyUSB1"
#define CRCERRORLOG "crc-error.log"
static FILE* crc_error_log = NULL;
static const UASDKbaudrate_t baud = UASDKBAUD(9600);
static UASDKuartdescriptor_t uartTx = UASDKUARTDEF0, uartRx = UASDKUARTDEF0;
static UASDKpkt_testgen_t pktsrc = { UASDKunibuf_initdef, NULL };
typedef struct {
    pcUASDKpkt_t pktref;
    int result;
} pkt_reader_context_t, *ppkt_reader_context_t;
typedef const pkt_reader_context_t *pcpkt_reader_context_t;

static void log_crc_error(pcUASDKpkt_t pkt, pcUASDKpkt_t pktref)
{
    fprintf(crc_error_log, "RAD(%02x,%02x), SAD(%02x,%02x), PLL(%04x,%04x) : ",
        pkt->rad, pktref->rad, pkt->sad, pktref->sad, pkt->opt.pll, pktref->opt.pll);
    uint16_t payload_length = pktref->opt.pll;
    uint16_t index_error = 0xffff;
    for (uint16_t i = 0; i < payload_length; i++)
    {
        if (pkt->payload[i] != pktref->payload[i])
        {
            index_error = i;
            break;
        }
    }
    int begin = __max((int)index_error - 8, 0);
    int end = __min((int)index_error + 8, (int)payload_length - 1);
    fprintf(crc_error_log, "payload[%d..%d]", begin, end);
    for (int j = begin; j <= end; j++)
    {
        fprintf(crc_error_log, ", (%02x,%02x)", pkt->payload[j], pktref->payload[j]);
    }
    fprintf(crc_error_log, "\n");
}
static void pkt_reader_callback(pcUASDKpkt_t rxoutpkt, void* context_)
{
    int crc = UASDKpkt_checkedc(rxoutpkt);
    pcpkt_reader_context_t context = (pcpkt_reader_context_t)context_;
    fprintf(stderr, "%s: rxoutpkt = (0x%02x, 0x%02x, 0x%04x: 0x%02x, 0x%02x):CRC=%d\n",
        __FUNCTION__,
        rxoutpkt->rad, rxoutpkt->sad, rxoutpkt->opt.pll, rxoutpkt->payload[0], rxoutpkt->payload[1], crc);
    if (crc)
    {
        log_crc_error(rxoutpkt, context->pktref);
    }
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
        if (EXIT_SUCCESS != (err = BLfile_open(&crc_error_log, CRCERRORLOG, "w")))
        {
            fprintf(stderr, "%s,%d,BLfile_open(,%s,)=%d(0x%08x)\n",
                __FUNCTION__, __LINE__, CRCERRORLOG, err, err);
            break;
        }
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
        if (crc_error_log)
        {
            fclose(crc_error_log);
            crc_error_log = NULL;
        }
    } while (0);
    return err;
}

/*!
\brief synchronized write of random packet
*/
int writesync(uint8_t rad, uint8_t sad, uint16_t payload_size)
{
    int err = EXIT_SUCCESS;
    const UASDKpkt_gensrc_t gensrc = { UASDKpkt_gentype_random, rad, sad, {payload_size}, { (const void*)0x82 }};
    do {
        if (payload_size > 250)
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
    { 0x84, 0x44, 0x0010 },
    { 0x94, 0x45, 0x0040 },
    { 0xa4, 0x46, 0x0080 },
    { 0xa4, 0x46, 0x00f0 },
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
        for (int ii = 0; ii < 10; ii++)
        {
            sleep(1);
            for (int i = 0; i < ARRAYSIZE(header_src); i++)
            {
                pcpkt_header_src_t p = &header_src[i];
                if (EXIT_SUCCESS != (err = writesync(p->rad, p->sad, p->payload_size)))
                {
                    UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
                }           
            }
        }
        sleep(2);
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