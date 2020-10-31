#include    <unistd.h>
#include    <termios.h>
#include    <stdio.h>
#include    <fcntl.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <math.h>
#include    <time.h>
#include    "UASDK_setup.h"


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
}


static void set_byteformat(struct termios* uart_opt, UASDK_byteformat_t byte_format)
{
    if (byte_format == UASDK_byteformat_N1 || byte_format == UASDK_byteformat_N2)
    {
        uart_opt->c_cflag &= ~PARENB; // disable parity
        uart_opt->c_iflag &= ~(INPCK | ISTRIP);
    }
    else
    {
        uart_opt->c_cflag |= PARENB; // enable parity
        if (byte_format == UASDK_byteformat_E1 || byte_format == UASDK_byteformat_E2)
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

int UASDK_setup(pcUASDK_setup_t setup, int fd)
{
    int err = EXIT_SUCCESS;
    do {
        struct termios uart_opt;
        if (EXIT_SUCCESS != tcgetattr(fd, &uart_opt))
        {
            err = errno;
            fprintf(stderr, "%s,%d,errno=%d(0x%04x)\n", __FILE__, __LINE__, err, err);
            break;
        }
        cfsetispeed(&uart_opt, setup->baudrate.id);
        cfsetospeed(&uart_opt, setup->baudrate.id);
        uart_opt.c_cflag |= (CLOCAL | CREAD);
        uart_opt.c_cflag |= ~CRTSCTS; // disable RTS/CTS control
        set_byteformat(&uart_opt, setup->byteformat);
        set_misc_options(&uart_opt);
        if (EXIT_SUCCESS != tcsetattr(fd, TCSANOW, &uart_opt))
        {
            err = errno;
            fprintf(stderr, "%s,%d,errno=%d(0x%04x)\n", __FILE__, __LINE__, err, err);
            break;
        }
    } while (0);
    return err;
}

pcUASDK_setup_t UASDK_default_setup()
{
    static const UASDK_setup_t setup = UASDK_SETUP_DEFAULT;
    return &setup;
}

int UASDK_open(const char* devicename, int* pfd)
{
    int err = EXIT_SUCCESS;
    do {
        *pfd = open(devicename, O_NDELAY | O_RDWR | O_NOCTTY);
        if (*pfd == -1)
        {
            err = errno;
            fprintf(stderr, "%s,%d,errno=%d(0x%04x), %s\n", __FILE__, __LINE__, err, err, devicename);
            break;
        }
        // read() returns immediately even if the read buffer has no characters.
        // see "Serial Programming Guide for POSIX Operating Systems," p.12.
        fcntl(*pfd, F_SETFL, O_NONBLOCK);
    } while (0);
    return err;
}

void UASDK_setup_estimate_time(pcUASDK_setup_t setup, int byte_count, struct timespec* time)
{
    static const int bits_per_byte[] =
    {
        10, 11, 11, 11, 12, 12
    };
    float total_period = setup->baudrate.bit_period * 
        (float)(bits_per_byte[setup->byteformat] * (int)byte_count);
    float int_part, fract_part;
    fract_part = modff(total_period, &int_part);
    time->tv_sec = (int)int_part;
    time->tv_nsec = (int)(fract_part * 1.0e9);
}