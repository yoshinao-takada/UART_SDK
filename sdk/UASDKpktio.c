#include    "sdk/UASDKpktio.h"
#include    <errno.h>

int UASDKpktio_writesync(pcUASDKuartdescriptor_t uart, pcUASDKpkt_t pkt)
{
    int err = EXIT_SUCCESS;
    do {
        size_t total_bytes = UASDKpkt_totalbytes(pkt);
        const UASDKunibuf_t txbuf = { { (void*)pkt, total_bytes, total_bytes } };
        int actually_written = 0;
        err = UASDKiobase_write(uart, &txbuf, &actually_written);
    } while (0);
    return err;
}

#define RXBUF_INIDEF { \
    UASDKunibuf_initdef, NULL, UASDKunibuf_initdef, NULL \
}

void UASDKpktio_rxbuf_reset(pUASDKpktio_rxbuf_t rxbuf)
{
    rxbuf->rxbuf0.voidbuf.filled_bytes = rxbuf->rxbuf2.voidbuf.filled_bytes = 0;
    rxbuf->rxbuf1->rdptr = rxbuf->rxbuf1->wrptr = 0;
}

void rxbuf_init(pUASDKpktio_rxbuf_t rxbuf)
{
    assert(EXIT_SUCCESS == UASDKunibuf_initbyte(&rxbuf->rxbuf0, 256));
    assert(EXIT_SUCCESS == UASDKunibuf_initbyte(&rxbuf->rxbuf2, 256));
    assert(EXIT_SUCCESS == BLringbuf_new(10, &rxbuf->rxbuf1));
    rxbuf->rxpkt = (pUASDKpkt_t)rxbuf->rxbuf2.voidbuf.buf;
}


typedef enum {
    StateID_empty,
    StateID_payload_incomplete,
    StateID_Null
} StateID_t;

struct UASDKpktio_state;

typedef StateID_t (*UASDKpktio_statehandler_t)
    (pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
typedef void (*UASDKpktio_actionhandler_t)
    (pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);

typedef struct UASDKpktio_state {
    UASDKpktio_statehandler_t state_handler;
    UASDKpktio_actionhandler_t enter_handler, exit_handler;
} UASDKpktio_state_t, *pUASDKpktio_state_t;

StateID_t SH_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
StateID_t SH_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
StateID_t SH_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) { return StateID_Null; }

void Enter_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
void Enter_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
void Enter_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}


void Exit_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
void Exit_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
void Exit_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}

static const UASDKpktio_state_t states[] =
{
    { SH_empty, Enter_empty, Exit_empty },
    { SH_payload_incomplete, Enter_payload_incomplete, Exit_payload_incomplete },
    { SH_Null, Enter_Null, Exit_Null }
};

static void* readthread(void* pvreader)
{
    pUASDKpktio_reader_t reader = (pUASDKpktio_reader_t)pvreader;
    pUASDKuartdescriptor_t uart = &reader->uart;
    UASDKpktio_rxbuf_t rxbuf = RXBUF_INIDEF;
    rxbuf_init(&rxbuf);
    StateID_t state = StateID_empty;
    states[state].enter_handler(&rxbuf, reader);
    int read_status = EXIT_SUCCESS;
    while (read_status == EXIT_SUCCESS)
    {
        if (read_status = UASDKiobase_read(uart, &rxbuf.rxbuf0))
        {
            UASDKpktio_rxbuf_reset(&rxbuf);
            continue;
        }
        uint16_t actual_put = 0;
        if (BLringbuf_put(rxbuf.rxbuf1, (uint16_t)rxbuf.rxbuf0.bytebuf.filled_bytes,
            rxbuf.rxbuf0.bytebuf.buf, &actual_put))
        {
            UASDKpktio_rxbuf_reset(&rxbuf);
            continue;
        }
        rxbuf.rxbuf0.bytebuf.filled_bytes = 0;
        while (BLringbuf_available_data(rxbuf.rxbuf1))
        {
            StateID_t next_state = states[state].state_handler(&rxbuf, reader);
            if (next_state == StateID_Null)
            {
                state = StateID_empty;
                states[state].enter_handler(&rxbuf, reader);
                break;
            }
            else if (next_state != state)
            {
                state = next_state;
            }
        }
    }
    return pvreader;
}

StateID_t SH_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader)
{
    StateID_t next_state = StateID_empty;
    do {
        uint16_t pkt_header_size = (uint16_t)sizeof(UASDKpkt_t);
        uint16_t actual_moved = 0;
        if (BLringbuf_available_data(rxbuf->rxbuf1) < pkt_header_size)
        { // do nothing
            break;
        }
        BLringbuf_get(rxbuf->rxbuf1, pkt_header_size, rxbuf->rxbuf2.bytebuf.buf, &actual_moved);
        rxbuf->rxbuf2.bytebuf.filled_bytes += actual_moved;
        if (rxbuf->rxpkt->opt.pll_sign < 0)
        { // payloadless packet was completed.
            reader->rxcallback(rxbuf->rxpkt, reader);
            Enter_empty(rxbuf, reader);
        }
        else
        {
            next_state = StateID_payload_incomplete;
        }
    } while (0);
    return next_state;
}
StateID_t SH_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader)
{
    StateID_t next_state = StateID_payload_incomplete;
    do {
        uint16_t available_byte_count = BLringbuf_available_data(rxbuf->rxbuf1);
        if (available_byte_count == 0)
        { // no data
            break;
        }
        uint16_t total_pkt_size = UASDKpkt_totalbytes(rxbuf->rxpkt);
        uint16_t required_byte_count = total_pkt_size - rxbuf->rxbuf2.bytebuf.filled_bytes;
        uint16_t moved_byte_count = __min(required_byte_count, available_byte_count);
        uint16_t actual_moved = 0;
        BLringbuf_get(rxbuf->rxbuf1, moved_byte_count, 
            rxbuf->rxbuf2.bytebuf.buf +  rxbuf->rxbuf2.bytebuf.filled_bytes, &actual_moved);
        rxbuf->rxbuf2.bytebuf.filled_bytes += actual_moved;
        if (total_pkt_size == (uint16_t)rxbuf->rxbuf2.bytebuf.filled_bytes)
        {
            next_state = StateID_empty;
            reader->rxcallback(rxbuf->rxpkt, reader);
            Enter_empty(rxbuf, reader);
        }
    } while (0);
    return next_state;
}

void Enter_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader)
{
    rxbuf->rxbuf2.bytebuf.filled_bytes = 0;
}

void Enter_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}


void Exit_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}
void Exit_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader)
{
}
