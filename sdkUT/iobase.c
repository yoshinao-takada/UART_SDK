#include    "base/BLbase.h"
#include    "base/BLts.h"
#include    "sdk/UASDKiobase.h"
#include    <string.h>
#include    <unistd.h>
#include    <pthread.h>
#include    <signal.h>
#define DEVICE0     "/dev/ttyUSB0"
#define DEVICE1     "/dev/ttyUSB1"

static int iobase_synch(); // synchronous calling read/write
static int iobase_thread(); // threading read and callback, synchronous calling write

int iobase()
{
    int err = EXIT_SUCCESS;
    do {
        int err_each = EXIT_SUCCESS;
        err |= err_each = iobase_synch();
        UT_SHOW(stderr, __FUNCTION__, __LINE__, err_each);
        err |= err_each = iobase_thread();
        UT_SHOW(stderr, __FUNCTION__, __LINE__, err_each);
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static const char*  data[] =
{
    "0123--4567++**//",
    "1234567890ABCDEFGHIJKLMNO",
    "abcdefghijklmnopqrstuvwxyz"
    "0123--4567++**//",
    "1234567890ABCDEFGHIJKLMNO",
    "abcdefghijklmnopqrstuvwxyz"
    "0123--4567++**//",
    "1234567890ABCDEFGHIJKLMNO",
    "abcdefghijklmnopqrstuvwxyz"
    "0123--4567++**//",
    "1234567890ABCDEFGHIJKLMNO",
    "abcdefghijklmnopqrstuvwxyz"
    "0123--4567++**//",
    "1234567890ABCDEFGHIJKLMNO",
    "abcdefghijklmnopqrstuvwxyz"
    "0123--4567++**//",
    "1234567890ABCDEFGHIJKLMNO",
    "abcdefghijklmnopqrstuvwxyz"
};

static const UASDKbaudrate_t baudrate = UASDKBAUD(9600);

void transfer_time(int index, struct timespec* t)
{
    int byte_count = (int)strlen(data[index]) + 4;
    UASDKbaudrate_estimatetime(&baudrate, byte_count, UASDKbyteformat_N1, t);
}

static int iobase_synch()
{
    int err = EXIT_SUCCESS;
    UASDKunibuf_t rxbuf  = UASDKunibuf_initdef;
    UASDKunibuf_t txbuf  = UASDKunibuf_initdef;
    UASDKuartdescriptor_t uartRx, uartTx;
    struct timespec t;
    do {
        if ((EXIT_SUCCESS != (err = UASDKunibuf_initbyte(&txbuf, 256))) ||
            (EXIT_SUCCESS != (err = UASDKunibuf_initbyte(&rxbuf, 256))))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = UASDKiobase_open(&uartTx, DEVICE0, &baudrate, UASDKbyteformat_N1)))
        {
            break;
        }
        if (EXIT_SUCCESS != (err = UASDKiobase_open(&uartRx, DEVICE1, &baudrate, UASDKbyteformat_N1)))
        {
            break;
        }
        for (int i = 0; i < ARRAYSIZE(data); i++)
        {
            transfer_time(i, &t);
            txbuf.voidbuf.filled_bytes = strlen(data[i]);
            memcpy(txbuf.voidbuf.buf, (const void*)data[i], txbuf.voidbuf.filled_bytes);
            if (EXIT_SUCCESS != (err = UASDKiobase_write(&uartTx, &txbuf)))
            {
                ERROR_LOGBR(__FUNCTION__, __LINE__, err);
            }
            nanosleep(&t, NULL);
            if (EXIT_SUCCESS != (err = UASDKiobase_read(&uartRx, &rxbuf)))
            {
                ERROR_LOGBR(__FUNCTION__, __LINE__, err);
            }
            if (rxbuf.bytebuf.filled_bytes > 0)
            {
                rxbuf.bytebuf.buf[rxbuf.bytebuf.filled_bytes] = '\0';
                fprintf(stderr, "read string = %s\n", rxbuf.bytebuf.buf);
                rxbuf.bytebuf.filled_bytes = 0;
            }
        }
        nanosleep(&t, NULL);
        if (EXIT_SUCCESS != (err = UASDKiobase_read(&uartRx, &rxbuf)))
        {
            ERROR_LOGBR(__FUNCTION__, __LINE__, err);
        }
        rxbuf.bytebuf.buf[rxbuf.bytebuf.filled_bytes] = '\0';
        fprintf(stderr, "read string = %s\n", rxbuf.bytebuf.buf);
    } while (0);
    UASDKunibuf_destroy(&txbuf);
    UASDKunibuf_destroy(&rxbuf);
    close(uartTx.fd);
    close(uartRx.fd);
    return err;
}

typedef struct {
    struct timespec reftime;
    pUASDKuartdescriptor_t uart;
} iobase_thread_context_t, *piobase_thread_context_t;

static void my_SIGINT_handler(int signal_number)
{
    if (signal_number != SIGINT)
    {
        return;
    }
    printf("SIGINT received.\n");
}

static void my_SIGHUP_handler(int signal_number)
{
    if (signal_number != SIGHUP)
    {
        return;
    }
    printf("SIGHUP received.\n");
}

static void my_SIGIO_handler(int signal_number)
{
    if (signal_number != SIGIO)
    {
        return;
    }
    printf("SIGIO received.\n");
}

void* readthread(void* pvcontext)
{
    sigset_t sigset;
    struct sigaction sa;
    sa.sa_handler = my_SIGIO_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGIO);
    sigaction(SIGIO, &sa, NULL);
    piobase_thread_context_t context = (piobase_thread_context_t)pvcontext;
    UASDKunibuf_t unibuf = UASDKunibuf_initdef;
    UASDKunibuf_initbyte(&unibuf, 256);
    BLts_logelapsedms(stderr, &context->reftime, "%s,calling UASDKiobase_read()\n", __FUNCTION__);
    int read_status = UASDKiobase_read(context->uart, &unibuf);
    BLts_logelapsedms(stderr, &context->reftime, "%s,returned %d from UASDKiobase_read()\n", __FUNCTION__, read_status);
    UASDKunibuf_destroy(&unibuf);
    return pvcontext;
}


static int iobase_thread()
{
    int err = EXIT_SUCCESS;
    UASDKuartdescriptor_t uart;
    const UASDKbaudrate_t baudrate = UASDKBAUD(9600);
    iobase_thread_context_t context = { { 0, 0 }, &uart };
    pthread_t thread;
    void* thread_return;
    do {
        err = UASDKiobase_open(&uart, DEVICE0, &baudrate, UASDKbyteformat_N1);
        if (err != EXIT_SUCCESS)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        BLts_getnow(&context.reftime);
        BLts_logelapsedms(stderr, &context.reftime, "%s,creating thread\n", __FUNCTION__);
        if (EXIT_SUCCESS != (err = pthread_create(&thread, NULL, readthread, (void*)&context)))
        {
            break;
        }
        BLts_logelapsedms(stderr, &context.reftime, "%s created thread.\n", __FUNCTION__);
        sleep(2);
        BLts_logelapsedms(stderr, &context.reftime, "%s killing thread.\n", __FUNCTION__);
        if (EXIT_SUCCESS != (err = pthread_kill(thread, SIGIO)))
        {
            break;
        }
        BLts_logelapsedms(stderr, &context.reftime, "%s signal(,SIGIO).\n", __FUNCTION__);
        if (EXIT_SUCCESS != (err = pthread_join(thread, &thread_return)))
        {
            break;
        }
        BLts_logelapsedms(stderr, &context.reftime, "%s joined thread.\n", __FUNCTION__);
    } while (0);
    return err;
}
