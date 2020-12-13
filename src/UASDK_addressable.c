#include    "UASDK_addressable.h"
#include    <errno.h>

int UASDK_addressable_new(uint8_t size, pUASDK_addressable_t* ppaddressable)
{
    int err = EXIT_SUCCESS;
    do {
        size_t cb_head = sizeof(UASDK_addressable_t);
        size_t cb_buf = (size_t)size * sizeof(UASDK_channel_t);
        pUASDK_addressable_t p = (*ppaddressable = (pUASDK_addressable_t)calloc(cb_buf + cb_head, 1));
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->channels = (pUASDK_channel_t)p->buf;
        p->size = size;
    } while (0);
    return err;
}

int UASDK_addressable_init(
    pUASDK_addressable_t addressable,
    uint8_t index,
    uint8_t address,
    UASDK_callback_t callback
) {
    int err = EXIT_SUCCESS;
    do {
        addressable->channels[index].address = address;
        addressable->channels[index].callback = callback;
        addressable->channels[index].packet_counter = 0;
    } while (0);
    return err;
}

int UASDK_addressable_device_new(
    uint8_t myaddress,
    uint8_t size,
    const char* name,
    pcUASDK_setup_t setup,
    pUASDK_addressable_device_t *ppdevice
) {
    int err = EXIT_SUCCESS;
    do {
        pUASDK_addressable_device_t device = 
        (*ppdevice = (pUASDK_addressable_device_t)calloc(1, sizeof(UASDK_addressable_device_t)));
        if (!device)
        {
            err = ENOMEM;
            break;
        }
        if (EXIT_SUCCESS != (err = UASDK_addressable_new(size, &device->addressable)))
        {
            break;
        }
        device->addressable->my_address = myaddress;
        if (EXIT_SUCCESS != (err = UASDK_commport_open(name, setup, &device->commport)))
        {
            break;
        }
    } while (0);
    if (err && *ppdevice)
    {
        free((void*)*ppdevice);
    }
    return err;
}

int UASDK_addressable_device_close(pUASDK_addressable_device_t *ppdevice)
{
    int err = EXIT_SUCCESS;
    do {
        free((void*)(*ppdevice)->addressable);
        err = UASDK_commport_close(&(*ppdevice)->commport);
        free((void*)*ppdevice);
        *ppdevice = NULL;
    } while (0);
    return err;
}