#include    "CRCTest/CRCTestLib.h"

static const uint16_t data[] = CRC_TEST_DATA;
static const uint16_t noerror[] = CRC_TEST_NOERROR;
static const uint16_t error0[] = CRC_TEST_ERROR0;
static const uint16_t error1[] = CRC_TEST_ERROR1;
static const uint16_t seed = CRC_SEED;

int main()
{
    uint16_t result_noerror = CRCTestRun(data, noerror, seed);
    uint16_t result_error0 = CRCTestRun(data, error0, seed);
    uint16_t result_error1 = CRCTestRun(data, error1, seed);

    printf("result_noerror = 0x%04x\n", result_noerror);
    printf("result_error0 = 0x%04x\n", result_error0);
    printf("result_error1 = 0x%04x\n", result_error1);

    return 0;
}