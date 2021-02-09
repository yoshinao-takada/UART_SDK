#include    <stdio.h>
#include    <unistd.h>
#include    <errno.h>
#include    <stdlib.h>
#include    "UASDK_thread.h"

#define PRINT_FLUSH(...) { \
    fprintf(stdout, __VA_ARGS__); fflush(stdout); \
}

int i;
void* callback0(void* pvparam)
{
    int* piparam = (int*)pvparam;
    PRINT_FLUSH("%s,%d,i = %d\n", __FUNCTION__, __LINE__, *piparam);
    (*piparam)++;
    sleep(3);
    PRINT_FLUSH("%s,%d\n", __FUNCTION__, __LINE__);
    return (void*) pvparam;
}

int main(int argc, const char* *argv)
{
    int err = EXIT_SUCCESS;
    pUASDK_thread_t thread = NULL;
    do {
        if (EXIT_SUCCESS != (err = UASDK_thread_new(&thread, callback0, (void*)&i)))
        {
            break;
        }
        sleep(1);
        for (int j = 0; j < 4; j++)
        {
            PRINT_FLUSH("%s,%d,j=%d\n", __FUNCTION__, __LINE__, j);
            UASDK_thread_enter_core_proc(thread);
            UASDK_thread_wait_core_proc_exit(thread);
            PRINT_FLUSH("%s,%d,j=%d\n", __FUNCTION__, __LINE__, j);
        }
    } while (0);
    if (thread)
    {
        UASDK_thread_delete(&thread);
    }
    return err;
}