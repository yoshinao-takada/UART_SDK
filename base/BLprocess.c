#include "base/BLprocess.h"
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#ifdef __linux__
#include <wait.h>
#endif
#include <unistd.h>
#include <errno.h>

int BLprocess_execv_sync(char* const argv[], int* exit_code)
{
    int err = EXIT_SUCCESS;
    do {
        HANDLE h_process = NULL;
        if (EXIT_SUCCESS != (err = BLprocess_execv(argv, &h_process)))
        {
            break;
        }
        err = BLprocess_wait_process(h_process, exit_code);
    } while (false);
    return err;
}

int BLprocess_execv(char* const argv[], HANDLE* ph_proc)
{
    int err = EXIT_SUCCESS;
    do {
        pid_t   pid = fork();
        if (pid == 0) 
        { // child process
            if  (-1 == execv(argv[0], argv))
            {
                err = errno;
                break;
            }
        }
        else
        { // parent process
            *ph_proc = (HANDLE)(intptr_t)pid;
        }        
    } while (false);
    return err;
}


int BLprocess_wait_process(HANDLE h_proc, int* exit_code)
{
    int err = EXIT_SUCCESS;
    pid_t pid = (pid_t)(intptr_t)h_proc;
    do {
        int status = 0;
        if (-1 == waitpid(pid, &status, 0))
        {
            err = errno;
            break;
        }
        if (WIFEXITED(status))
        {
            *exit_code = WEXITSTATUS(status);
        }
        else
        {
            *exit_code = 0;
            err = EFAULT;
        }        
    } while (false);
    return err;
}

