#ifndef CRCTESTLIB_H_
#define CRCTESTLIB_H_
#include    "base/BLcrc.h"
#include    "base/BLbase.h"

#define CRC_TEST_DATA { \
    0x0fc0, 0x1096, 0x5042, 0x0010, \
    0x7ff7, 0xf86a, 0xb58e, 0x7651, \
    0x8b88, 0x0679, 0x0123, 0x9599, \
    0xc58c, 0xd1e2, 0xe144, 0xb691 }
#define CRC_TEST_NOERROR { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }
#define CRC_TEST_ERROR0 { 0,0,0,0, 0,0,0,0, 0,0x0100,0,0, 0,0,0,0 }
#define CRC_TEST_ERROR1 { 0,0,0x0040,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }

#define CRC_SEED 0xffff

#define DATA_ARRAY_SIZE 16

#define CRC_TEST_DATA9 { 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 }

#define DATA9_ARRAY_SIZE 9
uint16_t CRCTestRun(const uint16_t* data, const uint16_t* error, uint16_t seed);
#endif /* CRCTESTLIB_H_ */