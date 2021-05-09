#include    "sdk/UASDKpktio.h"
#include    "sdk/UASDKpktioprivate.h"
#include    "sdk/UASDKpktiostate.h"
#include    <errno.h>
#include    <stdbool.h>
#include    <signal.h>
#include    <unistd.h>

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

void UASDKpktio_rxbuf_reset(pUASDKpktio_rxbuf_t rxbuf)
{
    rxbuf->rxbuf0.voidbuf.filled_bytes = rxbuf->rxbuf2.voidbuf.filled_bytes = 0;
    rxbuf->rxbuf1->rdptr = rxbuf->rxbuf1->wrptr = 0;
}

static void rxbuf_init(pUASDKpktio_rxbuf_t rxbuf)
{
    bool init_result =
        (EXIT_SUCCESS == UASDKunibuf_initbyte(&rxbuf->rxbuf0, 256)) &&
        (EXIT_SUCCESS == UASDKunibuf_initbyte(&rxbuf->rxbuf2, 256)) &&
        (EXIT_SUCCESS == BLringbuf_new(10, &rxbuf->rxbuf1));
    assert(init_result);
    rxbuf->rxpkt = (pUASDKpkt_t)rxbuf->rxbuf2.voidbuf.buf;
}

static const UASDKpktio_state_t states[] = STATES_DEF;
#pragma region state_machine_control

State_t SH_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader)
{
    fprintf(stderr, "%s() enter\n", __FUNCTION__);
    State_t next_state = { StateID_empty, Substate_continue };
    do {
        uint16_t pkt_header_size = (uint16_t)sizeof(UASDKpkt_t);
        uint16_t actual_moved = 0;
        if (BLringbuf_available_data(rxbuf->rxbuf1) < pkt_header_size)
        { // do nothing because of nodate
            next_state.substate = Substate_nodate;
            break;
        }
        BLringbuf_get(rxbuf->rxbuf1, pkt_header_size, rxbuf->rxbuf2.bytebuf.buf, &actual_moved);
        rxbuf->rxbuf2.bytebuf.filled_bytes += actual_moved;
        if (rxbuf->rxpkt->opt.pll_sign < 0)
        { // payloadless packet was completed.
            reader->rxcallback(rxbuf->rxpkt, reader->callback_context);
            Enter_empty(rxbuf, reader); // empty rxbuf.rxbuf2; rxbuf2 is the output stage packet buffer
        }
        else
        {
            next_state.state = StateID_payload_incomplete;
        }
    } while (0);
    return next_state;
}

State_t SH_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader)
{
    fprintf(stderr, "%s() enter\n", __FUNCTION__);
    State_t next_state = { StateID_payload_incomplete, Substate_continue };
    do {
        uint16_t available_byte_count = BLringbuf_available_data(rxbuf->rxbuf1);
        if (available_byte_count == 0)
        { // no data
            next_state.substate = Substate_nodate;
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
        { // packet with payload was complete
            next_state.state = StateID_empty;
            reader->rxcallback(rxbuf->rxpkt, reader->callback_context);
            Enter_empty(rxbuf, reader); // empty rxbuf.rxbuf2; rxbuf2 is the output stage packet buffer
        }
        else
        {
            next_state.substate = Substate_nodate;
        }
    } while (0);
    return next_state;
}

void Enter_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader)
{
    fprintf(stderr, "%s() enter\n", __FUNCTION__);
    rxbuf->rxbuf2.bytebuf.filled_bytes = 0;
}

void Enter_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}


void Exit_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}
void Exit_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}

State_t SH_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) 
{
    State_t nullstate = { StateID_Null, Substate_nodate };
     return nullstate;
}

void Enter_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}

void Exit_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader) {}
#pragma endregion state_machine_control

static void sigint_handler(int signum) {}

static int set_sigint_handler()
{
    int err = EXIT_SUCCESS;
    do {
        struct sigaction sa;
        sa.sa_flags = 0;
        sa.sa_handler = sigint_handler;
        if ((-1 == sigemptyset(&sa.sa_mask)) || (-1 == sigaddset(&sa.sa_mask, SIGINT)))
        {
            err = errno;
            break;
        }
        if (-1 == sigaction(SIGINT, &sa, NULL))
        {
            err = errno;
        }
    } while (0);
    return err;
}

static void* readthread(void* pvreader)
{
    pUASDKpktio_reader_t reader = (pUASDKpktio_reader_t)pvreader;
    pcUASDKuartdescriptor_t uart = reader->uart;
    UASDKpktio_rxbuf_t rxbuf = RXBUF_INIDEF;
    rxbuf_init(&rxbuf);
    State_t state = { StateID_empty, Substate_nodate };
    states[state.state].enter_handler(&rxbuf, reader);
    int read_status = set_sigint_handler();
    if (read_status) return pvreader;
    while (read_status == EXIT_SUCCESS)
    {
        reader->cmd = UASDKpktiocmd_ready;
        if (read_status = UASDKiobase_read(uart, &rxbuf.rxbuf0))
        {
            fprintf(stderr, "%s,%d, UASDKiobase_read() = %d\n", __FUNCTION__, __LINE__, read_status);
            UASDKpktio_rxbuf_reset(&rxbuf);
            if (read_status == EINTR && reader->cmd == UASDKpktiocmd_reset)
            {
                read_status = EXIT_SUCCESS;
            }
            continue;
        }
        fprintf(stderr, "%s,%d, UASDKiobase_read() = %d\n", __FUNCTION__, __LINE__, read_status);
        uint16_t actual_put = 0;
        if (BLringbuf_put(rxbuf.rxbuf1, (uint16_t)rxbuf.rxbuf0.bytebuf.filled_bytes,
            rxbuf.rxbuf0.bytebuf.buf, &actual_put))
        {
            UASDKpktio_rxbuf_reset(&rxbuf);
            continue;
        }
        fprintf(stderr, "%s,%d, BLringbuf_put(), bytes = %d\n", __FUNCTION__, __LINE__, actual_put);
        rxbuf.rxbuf0.bytebuf.filled_bytes = 0;
        state.substate = Substate_continue;
        do {
            State_t next_state = states[state.state].state_handler(&rxbuf, reader);
            if (next_state.state != state.state)
            {
                states[state.state].exit_handler(&rxbuf, reader);
                state.state = next_state.state;
                state.substate = next_state.substate;
                states[state.state].enter_handler(&rxbuf, reader);
            }
        } while (state.substate);
    } while (state.substate == Substate_continue);
    return pvreader;
}

// Step 1: reset reader
// Step 2: start readthread
// Step 3: wait for reader->cmd becoming UASDKpktiocmd_ready.
int UASDKpktio_start(pUASDKpktio_reader_t reader)
{
    int err = 0;
    do {
        // Step 1: reset reader
        reader->cmd = UASDKpktiocmd_none;
        // Step 2: start readthread
        if (EXIT_SUCCESS != (err = pthread_create(&reader->thread, NULL, readthread, (void*)reader)))
        {
            break;
        }
        // Step 3: wait for reader->cmd becoming UASDKpktiocmd_ready.
        struct timespec t = { 0, 1000 };
        do {
            nanosleep(&t, NULL);
        } while (reader->cmd != UASDKpktiocmd_ready);
    } while (0);
    return err;
}

int UASDKpktio_stop(pUASDKpktio_reader_t reader)
{
    int err = EXIT_SUCCESS;
    void* pvthreadreturn = NULL;
    do {
        if (reader->cmd != UASDKpktiocmd_ready)
        {
            err = EBUSY;
            break;
        }
        // set reader->cmd and send signal to readthread().
        reader->cmd = UASDKpktiocmd_stop;
        if (EXIT_SUCCESS != (err = pthread_kill(reader->thread, SIGINT)))
        {
            reader->cmd = UASDKpktiocmd_none;
            break;
        }
        // wait for readthread() termination.
        err = pthread_join(reader->thread, &pvthreadreturn);
    } while (0);
    return err;
}

int UASDKpktio_reset(pUASDKpktio_reader_t reader)
{
    int err = EXIT_SUCCESS;
    void* pvthreadreturn = NULL;
    do {
        if (reader->cmd != UASDKpktiocmd_ready)
        {
            err = EBUSY;
            break;
        }
        // set reader->cmd and send signal to readthread().
        reader->cmd = UASDKpktiocmd_reset;
        if (EXIT_SUCCESS != (err = pthread_kill(reader->thread, SIGINT)))
        {
            reader->cmd = UASDKpktiocmd_none;
            break;
        }
        // wait for readthread() termination.
        do {
            sleep(0);
        } while (reader->cmd != UASDKpktiocmd_ready);
    } while (0);
    return err;
}
