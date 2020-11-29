#ifndef UASDK_ADDRESSABLE_H_
#define UASDK_ADDRESSABLE_H_
#include    "UASDK_setup.h"
#include    "UASDK_thread.h"
#ifdef __cplusplus
#include    <cstdlib>
#include    <cstdint>
extern "C" {
#else
#include    <stdlib.h>
#include    <stdint.h>
#endif
typedef struct {
    uint8_t *your_addresses;
    uint16_t *packet_counters;
    UASDK_callback_t *callbacks;
    uint8_t size;
    uint8_t my_address;
    uint8_t buf[0]; // beginning of an extra memory region
} UASDK_addressable_t, *pUASDK_addressable_t;
typedef const UASDK_addressable_t *pcUASDK_addressable_t;

/*!
\brief create a new object
\param size [in]
\param addressable [out]
*/
int UASDK_addressable_new(uint8_t size, pUASDK_addressable_t* ppaddressable);

/*!
\brief initialize one of channels
\param index [in]
\param address [in]
\param callback [in]
*/
int UASDK_addressable_init(
    pUASDK_addressable_t addressable,
    uint8_t index,
    uint8_t address,
    UASDK_callback_t callback
);

typedef struct {
    pUASDK_commport_t commport;
    pUASDK_addressable_t addressable;
} UASDK_addressable_device_t, *pUASDK_addressable_device_t;
typedef const UASDK_addressable_device_t *pcUASDK_addressable_device_t;

/*!
\brief initialize a single address device object with a serialport interface
\param size [in] size of addressable part
\param name [in] serialport device name like "/dev/ttyS0", "/dev/ttyUSB0", etc.
\param setup [in] serialport setup info
\param ppdevice [out] newly created object
*/
int UASDK_addressable_device_new(
    uint8_t size,
    const char* name,
    pcUASDK_setup_t seteup,
    pUASDK_addressable_device_t *ppdevice
);

/*!
\brief close the device and release the allocated resouces.
\param ppdevice [in,out] the object to destroy
\return errno
*/
int UASDK_addressable_device_close(pUASDK_addressable_device_t *ppdevice);
#ifdef __cplusplus
}
#endif
#endif /* UASDK_ADDRESSABLE_H_ */