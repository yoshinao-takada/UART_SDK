#include "base/BLbase.h"
int pkt();
int iobase();
int pktgen();
int pktio();
int pktiostate();

int main()
{
    int err = EXIT_SUCCESS, err_each = EXIT_SUCCESS;
    //err |= (err_each = pkt());
    // err |= (err_each = iobase());
    // err |= (err_each = pktgen());
    //err |= (err_each = pktio());
    err |= (err_each = pktiostate());
    return err;
}