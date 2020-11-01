#include    <unistd.h>
#include    <termios.h>
#include    <stdio.h>
#include    <fcntl.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <strings.h>
#include    <string.h>
#include    <pthread.h>
#include    <time.h>
#include    <assert.h>
#include    "UASDK_setup.h"
#include    "UASDK_defs.h"
#include    "UASDK_buffer.h"
#include    "UASDK_ascii.h"
//#define UTEST
#define HELP_STRING \
"Command line format\n" \
"\ttest0 subcmd devname\n" \
"\t\tsubcmd : [c | s], \"c\" means client. \"s\" means server.\n" \
"\t\t\tA client initiating conversation, sends messages\n" \
"\t\t\tand receives replies.\n" \
"\t\t\tA server receives messages and replies.\n" \
"\t\tdevname : serial communication device name like \"/dev/ttyS0\",\n" \
"\t\t\t\"/dev/ttyUSB0\", etc.\n" \
"Command line examples\n" \
"\ttest0 s /dev/ttyS0\n" \
"\ttest0 c /dev/ttyUSB1\n"

#define ARGV_ID_SUBCMD      1
#define ARGV_ID_DEVNAME     2
#define SUBCMD_CLIENT       "c"
#define SUBCMD_SERVER       "s"
#define BUFFER_LENGTH       64

struct thread_arg {
    int fd;
    pcUASDK_setup_t setup;
    int quit_request;
    int client_input_digit;
} thread_arg = {
    -1, NULL, 0, 0
};

typedef struct thread_arg   *pthread_arg;

void* client_thread(void* arg)
{
    pthread_arg arg_ = (pthread_arg)arg;
    pUASDK_buffer_t buffer = NULL;
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = UASDK_buffer_new(BUFFER_LENGTH, &buffer)))
        {
            break;
        }
        for (;;)
        {
            buffer->caplen.byte_filled = (uint8_t)(
                1 + /* null termination length */
                snprintf(
                    buffer->buf, (size_t)buffer->caplen.byte_capacity,
                    "Client n=%d", arg_->client_input_digit));
            arg_->client_input_digit = (arg_->client_input_digit + 1) % 10;
            printf("buffer filled = %d, buffer strlen + 1 = %d, buffer string = %s\n",
                buffer->caplen.byte_filled, strlen(buffer->head.pstring)+1, buffer->head.pstring);
            err = UASDK_ascii_write(arg_->fd, buffer, arg_->setup);
            if (EXIT_SUCCESS != err)
            {
                break;
            }
            fflush(stdout);
            if (arg_->quit_request)
            {
                arg_->quit_request = EXIT_SUCCESS;
                break;
            }
            sleep(1);
        }
    } while (0);
    if (buffer)
    {
        free((void*)buffer);
    }
    arg_->quit_request = err;
    return arg;
}

void* server_thread(void* arg)
{
    pthread_arg arg_ = (pthread_arg)arg;
    pUASDK_buffer_t buffer = NULL, subbuffer = NULL;
    int err = EXIT_SUCCESS;
    struct timespec sleep_time;
    UASDK_setup_estimate_time(arg_->setup, 16, &sleep_time);
    do {
        if ((EXIT_SUCCESS != (err = UASDK_buffer_new(BUFFER_LENGTH, &buffer))) ||
            (EXIT_SUCCESS != (err = UASDK_buffer_new(BUFFER_LENGTH, &subbuffer))))
        {
            break;
        }
        int i = 0;
        for (;i != 50;)
        {
            int err_and_sts = UASDK_ascii_read(arg_->fd, buffer);
            int err = err_and_sts & 0xffff;
            int sts = err_and_sts & (UASDK_ascii_rlen_sts_bufferfull | UASDK_ascii_rnc_sts_1);
            if (EXIT_SUCCESS != err)
            {
                fprintf(stderr, "%s,%d,err=%d(0x%04x)\n", __FILE__, __LINE__, err, err);
            }
            else if (sts)
            {
                err = UASDK_ascii_shiftout(buffer,subbuffer);
                printf("%d:%s\n", i++, subbuffer->head.pstring);
                UASDK_buffer_clear(subbuffer);
            }            
            nanosleep(&sleep_time, NULL);
        }
    } while (0);
    if (buffer)
    {
        free((void*)buffer);
    }
    if (subbuffer)
    {
        free((void*)subbuffer);
    }
    arg_->quit_request = err;
    return arg;
}

#ifndef UTEST
int main(int argc, char* argv[])
{
    int err = EXIT_SUCCESS;
    thread_arg.setup = UASDK_default_setup();
    do {
        if (argc < 3)
        {
            err = EINVAL;
            fprintf(stderr, "%s", HELP_STRING);
            break;
        }
        if (EXIT_SUCCESS != (err = UASDK_open(argv[ARGV_ID_DEVNAME], &thread_arg.fd)))
        {
            break;
        }
        printf("tty device %s was opened. fd = %d\n", argv[ARGV_ID_DEVNAME], thread_arg.fd);
        if (EXIT_SUCCESS != (err = UASDK_setup(thread_arg.setup, thread_arg.fd)))
        {
            break;
        }
        printf("tty setup success\n");
        if (0 == strcasecmp(argv[ARGV_ID_SUBCMD], SUBCMD_CLIENT))
        { // client mode operation
            int continue_operation = 1;
            printf("Client operation started.\n");
            pthread_t thread;
            err = pthread_create(&thread, NULL, client_thread, &thread_arg);
            if (err)
            {
                printf("%s,%d,err=%d(0x%04x)\n", err);
                break;
            }
            else
            {
                sleep(1);
            }
            do {
                char cmd_buf[16];
                printf("Put \"q\" to quit or 1 digit number to continue -- ");
                fgets(cmd_buf, ARRAYSIZE(cmd_buf), stdin);
                if (0 == strncasecmp("q", cmd_buf, 1))
                {
                    thread_arg.quit_request = 1;
                    continue_operation--;
                }
                else
                {
                    int n = (int)(cmd_buf[0] - '0');
                    if (n < 0 || n > 9)
                    {
                        printf("Invalid input: %s", cmd_buf);
                    }
                    else
                    {
                        thread_arg.client_input_digit = n;
                        printf("thread_arg.client_input_digit was updated with %d.\n",
                            thread_arg.client_input_digit);
                    }
                }
            } while (continue_operation);
            void* thread_return = NULL;
            pthread_join(thread, &thread_return);
            printf("After pthread_join(), quit_request = %d\n", thread_arg.quit_request);
        }
        else if (0 == strcasecmp(argv[ARGV_ID_SUBCMD], SUBCMD_SERVER))
        { // server operation
            printf("Server operation started.\n");
            pthread_t thread;
            err = pthread_create(&thread, NULL, server_thread, &thread_arg);
            if (err)
            {
                printf("%s,%d,err=%d(0x%04x)\n", err);
                break;
            }
            else
            {
                sleep(1);
            }
            void* thread_return;
            pthread_join(thread, &thread_return);
            printf("After pthread_join(), quit_request = %d\n", thread_arg.quit_request);
        }
        else
        {
            err = EINVAL;
            fprintf(stderr, "%s,%d,subcmd = %s\n", __FILE__, __LINE__, argv[ARGV_ID_SUBCMD]);
            break;
        }        
    } while (0);
    if (thread_arg.fd > 0)
    {
        close(thread_arg.fd);
    }
    return err;
}
#else
int main(int argc, char* argv[])
{
    static const char src_string0[] = "abc";
    static const char src_string1[] = "def\0ghi";
    pUASDK_buffer_t buffer = NULL, subbuffer = NULL;
    int err = EXIT_SUCCESS;
    do {
        if ((EXIT_SUCCESS != (err = UASDK_buffer_new(64, &buffer))) ||
            (EXIT_SUCCESS != (err = UASDK_buffer_new(64, &subbuffer))))
        {
            fprintf(stderr, "%s,%d,err=%d(0x%04x)\n", __FILE__, __LINE__, err, err);
            break;
        }
        memcpy(buffer->head.pstring, src_string0, ARRAYSIZE(src_string0));
        buffer->caplen.byte_filled = ARRAYSIZE(src_string0);
        assert(4 == buffer->caplen.byte_filled);
        assert(0 == strcmp(src_string0, buffer->head.pstring));
        if (EXIT_SUCCESS != (err = UASDK_ascii_shiftout(buffer, subbuffer)))
        {
            fprintf(stderr, "%s,%d,err=%d(0x%04x)\n", __FILE__, __LINE__, err, err);
            break;
        }
        assert(4 == subbuffer->caplen.byte_filled);
        assert(0 == strcmp(src_string0, subbuffer->head.pstring));
        assert(0 == buffer->caplen.byte_filled);
        printf("subbuffer string = %s\n", subbuffer->head.pstring);
        memcpy(buffer->head.pstring, src_string1, ARRAYSIZE(src_string1));
        buffer->caplen.byte_filled = ARRAYSIZE(src_string1);
        assert(8 == buffer->caplen.byte_filled);
        UASDK_buffer_clear(subbuffer);
        if (EXIT_SUCCESS !=(err = UASDK_ascii_shiftout(buffer, subbuffer)))
        {
            fprintf(stderr, "%s,%d,err=%d(0x%04x)\n", __FILE__, __LINE__, err, err);
            break;
        }
        assert(4 == subbuffer->caplen.byte_filled);
        assert(0 == strcmp(src_string1, subbuffer->head.pstring));
        assert(4 == buffer->caplen.byte_filled);
        printf("subbuffer string = %s\n", subbuffer->head.pstring);
        UASDK_buffer_clear(subbuffer);
        if (EXIT_SUCCESS !=(err = UASDK_ascii_shiftout(buffer, subbuffer)))
        {
            fprintf(stderr, "%s,%d,err=%d(0x%04x)\n", __FILE__, __LINE__, err, err);
            break;
        }
        assert(4 == subbuffer->caplen.byte_filled);
        assert(0 == strcmp(src_string1+4, subbuffer->head.pstring));
        assert(0 == buffer->caplen.byte_filled);
        printf("subbuffer string = %s\n", subbuffer->head.pstring);
        printf("UTEST:main() test pass\n");
    } while (0);
    if (buffer)
    {
        free(buffer);
    }
    if (subbuffer)
    {
        free(subbuffer);
    }
    return err;
}
#endif