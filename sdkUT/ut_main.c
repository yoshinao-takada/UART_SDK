#include "base/BLbase.h"
int pkt();
int iobase();

int main()
{
    int err = EXIT_SUCCESS, err_each = EXIT_SUCCESS;
    err |= (err_each = pkt());
    err |= (err_each = iobase());
    return err;
}