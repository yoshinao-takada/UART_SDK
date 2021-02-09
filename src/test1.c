#include "UASDK_packet.h"
#include "base/BLthread.h"
#include "base/BLts.h"
#include    <memory.h>

#define PACKET_HEADER0  { 0x10, 0x11, 0x13f5, 0x02, 0, 0 }
#define PACKET_HEADER1  { 0x11, 0xfe, 0xfeff, 0x03, 0x40, 0 }

int UT_packet_struct_size()
{
    int err = EXIT_SUCCESS;
    do {
        size_t header_size = sizeof(UASDK_packet_header_t);
        size_t packet_size = sizeof(UASDK_packet_t);
        if (header_size != 8)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
        if (packet_size != 8)
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
        }
    } while (0);
    return err;
}
int UT_packet_header()
{
    int err = EXIT_SUCCESS;
    do {
        {
            UASDK_packet_header_t header = PACKET_HEADER0;
            if (EXIT_FAILURE != UASDK_packet_header_check_edc(&header))
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
            }
            UASDK_packet_header_set_edc(&header);
            if (EXIT_SUCCESS != UASDK_packet_header_check_edc(&header))
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
            }
        } {
            UASDK_packet_header_t header = PACKET_HEADER1;
            if (EXIT_FAILURE != UASDK_packet_header_check_edc(&header))
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
            }
            UASDK_packet_header_set_edc(&header);
            if (EXIT_SUCCESS != UASDK_packet_header_check_edc(&header))
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
            }
        } {
            const uint16_t payload_buffer_byte_count = 128; // payload buffer size including checksum region
            pUASDK_packet_buffer_t buffer = NULL;
            const UASDK_packet_header_t header = PACKET_HEADER0;
            do {
                if (EXIT_SUCCESS != UASDK_packet_buffer_new(payload_buffer_byte_count, &buffer))
                {
                    UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
                }
                if (buffer->payload_buffer_byte_count != payload_buffer_byte_count)
                {
                    UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
                }
                memcpy(&buffer->packet.header, &header, sizeof(UASDK_packet_header_t));
                if (EXIT_SUCCESS == UASDK_packet_header_check_edc(&buffer->packet.header))
                {
                    UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
                }
                // packet checksum test
                srand(1);
                buffer->packet.header.pll = payload_buffer_byte_count;
                for (uint16_t i = 0; i < (payload_buffer_byte_count - 2); i++)
                {
                    buffer->packet.payload[i] = (uint8_t)rand();
                }
                if (EXIT_SUCCESS == UASDK_packet_check_edc(&buffer->packet))
                {
                    UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err = EXIT_FAILURE);
                }
                UASDK_packet_set_edc(&buffer->packet);
            } while (0);
            BLSAFEFREE(&buffer);
        }
    } while (0);
    return err;
}

/*
[How to use BLthread]
1. core_proc()
It must be a short run-to-completion function and does not have to consider synchronization.
Synchronization is the role of thread_runner() which is a local function hidden in BLthread.c.
core_proc() does only something short and soon returns.

2. thread_sync()
It controls iteration and synchronization. It lets BLthread framework execute core_proc() repeatedly.
BLthread framework is used as described below.
Step 1: BLthread_new() create a thread executing thread_runner() as a thread.
  The synchronization objects, thread->sync_gate_enter, sync_gate_exit are locked in the initial state.
  thread object is declared as a pointer. The pointer is freed at the end of the program.
  thread_runner() is stop just before calling core_proc() until thread->sync_gate_enter is unlocked.
Step 2, 3 is in a loop.
Step 2: BLthread_enter_core_proc() unlocks thread->sync_gate_enter. The main thread waits on thread->sync_gate_exit
  until the completion of core_proc().
  The request parameter is BLthread_request_last_enter only when the last iteration count. This operation
  notify thread_runner to exit its own loop and terminates the thread.
Step 3: thread_runner() unlocks thread->sync_gate_exit after core_proc() returns. Then the main thread returns
  from BLthread_wait_core_proc_exit().
for-loop exits when param.count == iteration.
Step 4: BLthread_delete() waits for the thread termination by pthread_join() API. And then release resources, i.e.
  two mutexes and thread struct itself.
*/

typedef struct {
    struct timespec sleep_request;
    struct timespec sleep_remaining;
    int count;
} CoreProcParam_t, *pCoreProcParam_t;

// sleep for a period of time and return
static void* core_proc(void* param)
{
    pCoreProcParam_t param_ = (pCoreProcParam_t)param;
    nanosleep(&param_->sleep_request, &param_->sleep_remaining);
    param_->count++;
    return param;
}

int UT_thread_sync()
{
    static const int iteration = 4;
    int err = EXIT_SUCCESS;
    pBLthread_t thread = (pBLthread_t)NULL;
    CoreProcParam_t param =
    {
        { 2, 400000000 }, // sleep request time = 2.4 sec
        { 0, 0 }, // nanosleep() working area
        0 // count is incremented at each call of core_proc().
    };

    do {
        if (EXIT_SUCCESS != (err = BLthread_new(&thread, core_proc, (void*)&param)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        for (param.count = 0; param.count < iteration; /* param.count is incremented by core_proc() */)
        {
            struct  timespec ts0;
            BLts_getnow(&ts0);
            // (iteration - 1) is the last runnable count. At this count, BLthread_request_last_enter notification
            // is sent to thread_runner, requesting thread termination.
            if (EXIT_SUCCESS != (err = BLthread_enter_core_proc(
                thread, param.count == (iteration - 1) ? BLthread_request_last_enter : BLthread_request_none)))
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
            }
            if (EXIT_SUCCESS != (err = BLthread_wait_core_proc_exit(thread)))
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
            }
            BLts_logelapsedms(stderr, &ts0, "count=%d\n", param.count);
        }
    } while (0);
    err = BLthread_delete(&thread);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

int main(int argc, const char* argv[])
{
    int err = EXIT_SUCCESS;
    int item_err = EXIT_SUCCESS;
    do {
        err |= (item_err = UT_packet_struct_size());
        UT_SHOW(stdout, "UT_packet_struct_size", 0, item_err);
        err |= (item_err = UT_packet_header());
        UT_SHOW(stdout, "UT_packet_header", 0, item_err);
        err |= (item_err = UT_thread_sync());
        UT_SHOW(stdout, "UT_thread_sync", 0, item_err);
    } while (0);
    return err;
}