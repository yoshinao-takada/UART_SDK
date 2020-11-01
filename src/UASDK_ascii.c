#include    "UASDK_ascii.h"
#include    <unistd.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <time.h>
#include    <math.h>
#include    <memory.h>

static int contain_null(const uint8_t* begin, ssize_t length)
{
    int result = 0;
    const uint8_t *end = begin + length;
    for (; begin != end; begin++)
    {
        if (*begin == 0)
        {
            result++;
            break;
        }
    }
    return result;
}

int UASDK_ascii_read(int fd, pUASDK_buffer_t buffer)
{
    int err = EXIT_SUCCESS;
    int sts = 0;
    do {
        //-------------- read data --------------------
        ssize_t s = read(fd,
            buffer->head.pbytes + buffer->caplen.byte_filled, 
            UASDK_CAPLEN_AVAILABLE(buffer->caplen));
        if (s == -1)
        {
            if (EAGAIN == errno)
            { // Rx buffer empty
                break;
            }
            else
            { // unexpected error
                err = errno;
                break;
            }            
        }
        if ((int)s > UASDK_CAPLEN_AVAILABLE(buffer->caplen))
        { // too long read length (it shall never occur.)
            err = EINVAL;
            break;
        }
        //-------------- update sts -----------------
        else if (s == 0)
        {
            sts = UASDK_ascii_rlen_sts_nodata | UASDK_ascii_rnc_sts_0;
            break;
        }
        else
        {
            sts = UASDK_ascii_rlen_sts_normal |
                (contain_null(buffer->head.pbytes + buffer->caplen.byte_filled, s) ?
                UASDK_ascii_rnc_sts_1 : UASDK_ascii_rnc_sts_0);
        }       
        buffer->caplen.byte_filled += (uint8_t)s;
    } while (0);
    return err | sts;
}

int UASDK_ascii_write(int fd, pcUASDK_buffer_t buffer, pcUASDK_setup_t setup)
{
    int err = EXIT_SUCCESS;
    struct timespec sleep_request;
    UASDK_setup_estimate_time(setup, (int)buffer->caplen.byte_filled, &sleep_request);
    do {
        int not_completed = 1;
        do {
            ssize_t s = write(fd, buffer->head.pbytes, (size_t)buffer->caplen.byte_filled);
            if (s == -1)
            {
                if (EAGAIN != errno)
                { // unexpected error
                    err = errno;
                    not_completed = 0;
                }
                else
                { // communication buffer insufficient
                    nanosleep(&sleep_request, NULL);
                }
            }
            else
            {
                not_completed = 0; // write complete
            }
        } while (not_completed);
    } while (0);
    return err;
}

int UASDK_ascii_1st_strlen(pcUASDK_buffer_t buffer)
{
    int length = 0;
    const char* i_buffer = buffer->head.pstring; // buffer scan iterator
    const char* end_buffer = i_buffer + buffer->caplen.byte_filled; // end of buffer scan
    for (; i_buffer != end_buffer; i_buffer++)
    {
        if (*i_buffer == '\0')
        {
            break;
        }
        length++;
    }
    if (*i_buffer == '\0')
    { // including NULL termination
        length++;
    }
    return length;
}

int UASDK_ascii_shiftout(pUASDK_buffer_t in, pUASDK_buffer_t out)
{
    int err = EXIT_SUCCESS;
    do {
        //---------- check if copy operation is needed. --------------------
        int copy_length = UASDK_ascii_1st_strlen(in);
        if (copy_length == 0)
        { // no data to shift out
            *(out->head.pstring) = '\0';
            out->caplen.byte_filled = 0;
            break;
        }
        else if (copy_length > out->caplen.byte_capacity)
        { // out does not have a sufficient capacity
            err = ENOBUFS;
            break;
        }
        //----------- copy operation ------------
        memcpy(out->head.pstring, in->head.pstring, copy_length);
        out->caplen.byte_filled = copy_length;
        //----------- check if shift operation is needed ------------
        if (copy_length == in->caplen.byte_filled)
        { //-------- clear operation because all the bytes in 'in' was copied out.
            *(in->head.pstring) = '\0';
            in->caplen.byte_filled = 0;
            break;
        }
        //------------ shift operation ---------------
        char* i_dst = in->head.pstring;
        const char* i_src = i_dst + copy_length;
        int move_length = in->caplen.byte_filled - copy_length;
        if (*i_src == '\0')
        {
            i_src++;
            move_length--;
        }
        in->caplen.byte_filled = move_length;
        do {
            *i_dst++ = *i_src++;
        } while (--move_length);
    } while (0);
    return err;
}
