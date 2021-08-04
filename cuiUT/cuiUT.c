#include "cui/BLmultiwinfw.h"
#define WINCOUNT        4
#define LOGDIR          "./log"
#define DEFAULT_BUFFER_SIZE 128
static const char* TITLES[] = { "TOUCH_PANEL", "MOTOR1", "HEATER1", "REPORT" };

int main(int argc, const char* argv[])
{
    int err = EXIT_SUCCESS;
    do {
        if (EXIT_SUCCESS != (err = BLmultiwinfw_init(4, LOGDIR)))
        {
            UT_SHOWBREAK(stderr, __FILE__, __LINE__, err);
        }
        for (int i = 0; i < _countof(TITLES); ++i)
        {
            if (EXIT_SUCCESS != (err = BLmultiwinfw_initsub(i, TITLES[i], BLsubwin_loglevel_trace, DEFAULT_BUFFER_SIZE)))
            {
                UT_SHOWBREAK(stderr, __FILE__, __LINE__, err);
            }
        }
        
    } while (0);
    return err;
}