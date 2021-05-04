#include    "sdk/UASDKiobase.h"
#include    "base/BLbase.h"
#include    <termios.h>
#include    <unistd.h>
#include    <stddef.h>
#include    <sys/stat.h>
#include    <fcntl.h>
#include    <math.h>
#include    <memory.h>
#include    <signal.h>
#pragma region baudrate
static const int bits_per_byte[] =
{
    10, 11, 11, 11, 12, 12
};

void UASDKbaudrate_estimatetime(
    pcUASDKbaudrate_t baudrate, int byte_count, UASDKbyteformat_t format, struct timespec* time
) {
    float total_time = (float)(bits_per_byte[format] * byte_count) * baudrate->bittime;
    float int_part, fract_part;
    fract_part = modff(total_time, &int_part);
    time->tv_sec = (time_t)int_part;
    time->tv_nsec = (__syscall_slong_t)(1.0e9f * fract_part);
}
#pragma endregion baudrate
#pragma region unifiled_buffer
int UASDKunibuf_initbyte(pUASDKunibuf_t ub, uint16_t total_byte_count)
{
    assert(ub->voidbuf.buf == NULL);
    int err = EXIT_SUCCESS;
    do {
        void* pv = (UASDKunibuf_void(ub) = malloc(total_byte_count));
        if (!pv)
        {
            err = ENOMEM;
            break;
        }
        UASDKunibuf_byte_count(ub) = (size_t)total_byte_count;
    } while (0);
    return err;
}

int UASDKunibuf_extract(pUASDKunibuf_t ub, uint16_t ext_bytes, void* outbuf)
{
    int err = EXIT_SUCCESS;
    do {
        if (ext_bytes > ub->bytebuf.filled_bytes)
        {
            err = ENODATA;
            break;
        }
        memcpy(outbuf, ub->voidbuf.buf, (size_t)ext_bytes);
        size_t move_byte_count = ub->bytebuf.filled_bytes - (size_t)ext_bytes;
        if (move_byte_count > 0)
        {
            const void* src = (void*)(ub->bytebuf.buf + ext_bytes);
            memcpy(ub->voidbuf.buf, src, move_byte_count);
        }
        ub->bytebuf.filled_bytes -= ext_bytes;
    } while (0);
    return err;
}

void UASDKunibuf_destroy(pUASDKunibuf_t ub)
{
    BLSAFEFREE(&UASDKunibuf_void(ub));
    UASDKunibuf_byte_count(ub) = 0;
}
#pragma endregion unifiled_buffer

#pragma region init_uart
static void set_misc_options(struct termios* uart_opt)
{
    // disable canonical input (i.e. raw mode)
    // disable echo normal characters and echo erase characters as BS-SP-BS
    // disable SIGINTR, SIGSUSP, SIGDSUSP, SIGQUIT signals
    uart_opt->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // dispable X-ON, X-OFF, and any other character sequence flow control
    uart_opt->c_iflag &= ~(IXON | IXOFF | IXANY);
    // disable postprocess outout
    uart_opt->c_oflag &= ~OPOST;
    uart_opt->c_cflag |= (CLOCAL | CREAD);
    uart_opt->c_cflag &= ~CRTSCTS; // disable RTS/CTS control
}


static void set_byteformat(struct termios* uart_opt, UASDKbyteformat_t byte_format)
{
    if (byte_format == UASDKbyteformat_N1 || byte_format == UASDKbyteformat_N2)
    {
        uart_opt->c_cflag &= ~PARENB; // disable parity
        uart_opt->c_iflag &= ~(INPCK | ISTRIP);
    }
    else
    {
        uart_opt->c_cflag |= PARENB; // enable parity
        if (byte_format == UASDKbyteformat_E1 || byte_format == UASDKbyteformat_E2)
        {
            uart_opt->c_cflag &= ~PARODD;   
        }
        else
        {
            uart_opt->c_cflag |= PARODD;
        }        
        uart_opt->c_iflag |= (INPCK | ISTRIP);
    }
    uart_opt->c_cflag &= ~CSIZE;
    uart_opt->c_cflag |= CS8;
}

int UASDKiobase_open(pUASDKuartdescriptor_t uart, const char* name, pcUASDKbaudrate_t br, UASDKbyteformat_t bf)
{
    assert(bf == UASDKbyteformat_N1);
    int err = EXIT_SUCCESS;
    do {
        uart->fd = open(name, O_RDWR | O_NOCTTY);
        if (uart->fd == -1)
        {
            err = errno;
            fprintf(stderr, "%s,%d,error in open(%s,), err=%d(0x%04x)\n",
                __FILE__, __LINE__, name, err, err);
        }
        struct termios iosetup;
        if (EXIT_SUCCESS != tcgetattr(uart->fd, &iosetup))
        {
            err = errno;
            fprintf(stderr, "%s,%d,error in tcgetattr(), err=%d(0x%04x)\n",
                __FILE__, __LINE__, err, err);
        }
        cfsetispeed(&iosetup, br->id);
        cfsetospeed(&iosetup, br->id);
        set_misc_options(&iosetup);
        set_byteformat(&iosetup, bf);
        iosetup.c_cc[VTIME] = 0;
        iosetup.c_cc[VMIN] = 1;
        if (tcsetattr(uart->fd, TCSAFLUSH, &iosetup))
        {
            err = errno;
            fprintf(stderr, "%s,%d,error in tcsetattr(), err=%d(0x%04x)\n",
                __FILE__, __LINE__, err, err);
        }
        uart->byteformat = bf;
        memcpy(&uart->baudrate, br, sizeof(UASDKbaudrate_t));
    } while (0);
    return err;
}

int UASDKiobase_write(pcUASDKuartdescriptor_t uart, pcUASDKunibuf_t ub, int* actually_written)
{
    int err = EXIT_SUCCESS;
    do {
        ptrdiff_t offset = 0;
        while (offset != ub->voidbuf.filled_bytes)
        {
            ssize_t result = write(uart->fd, ub->bytebuf.buf + offset, ub->bytebuf.filled_bytes - offset);
            if (result >= 0)
            {
                offset += result;
            }
            if (offset != ub->voidbuf.filled_bytes)
            {
                struct timespec t = { 0, 0 };
                int wait_byte_count = (int)ub->voidbuf.filled_bytes - (int)offset;
                UASDKbaudrate_estimatetime(&uart->baudrate, wait_byte_count, uart->byteformat, &t);
                nanosleep(&t, NULL);
            }
        }
        fsync(uart->fd);
    } while (0);
    return err;
}

int UASDKiobase_read(pcUASDKuartdescriptor_t uart, pUASDKunibuf_t ub)
{
    int err = EXIT_SUCCESS;
    do {
        void* read_ptr = (void*)(ub->bytebuf.buf + ub->bytebuf.filled_bytes);
        size_t available_space = ub->bytebuf.byte_count - ub->bytebuf.filled_bytes;
        ssize_t result = read(uart->fd, read_ptr, available_space);
        if (result >= 0)
        {
            ub->voidbuf.filled_bytes += result;
        }
        else
        {
            err = errno;
        }
    } while (0);
    return err;
}

static void interrupt_handler(int signum)
{
    return;
}

int UASDKiobase_enableinterrupt(int signum)
{
    int err = EXIT_SUCCESS;
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = interrupt_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, signum);
    if (sigaction(signum, &sa, NULL))
    {
        err = errno;
    }
    return err;
}
#pragma endregion init_uart