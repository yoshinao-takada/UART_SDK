#include    "base/BLbase.h"
#include    "sdk/UASDKpacket.h"
#include    <memory.h>

static const UASDKpacket_header_t error_bit0 =
{ 0, 0, 0x0800, 0, 0, 0 };
static const UASDKpacket_header_t error_bit1 =
{ 0, 0, 0x0550, 0, 0, 0x0040 };
static const UASDKpacket_header_t test_header0 = 
{ 0x55, 0xaa, 0x1234, (uint8_t)UASDKpackettype_ACK, 0, 0 };
static const UASDKpacket_header_t test_header1 =
{ 0x55, 0xaa, 0x1234, (uint8_t)UASDKpackettype_request, 32, 0 };

static int packet_header_crc()
{
    int err = EXIT_SUCCESS;
    do {
        UASDKpacket_header_t test_header = test_header0;
        UASDKpacket_header_setedc(&test_header);

        if (EXIT_SUCCESS != (err = UASDKpacket_header_checkedc(&test_header)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }

        uint8_t* data_ptr = (uint8_t*)&test_header;
        const uint8_t* err_ptr = (const uint8_t*)&error_bit0;
        for (int i = 0; i < sizeof(UASDKpacket_header_t); i++)
        {
            data_ptr[i] ^= err_ptr[i];
        }
        if (EXIT_SUCCESS == UASDKpacket_header_checkedc(&test_header))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }

        err_ptr = (const uint8_t*)&error_bit1;
        for (int i = 0; i < sizeof(UASDKpacket_header_t); i++)
        {
            data_ptr[i] ^= err_ptr[i];
        }
        if (EXIT_SUCCESS == UASDKpacket_header_checkedc(&test_header))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }

        UASDKpacket_header_setedc(&test_header);

        if (EXIT_SUCCESS != (err = UASDKpacket_header_checkedc(&test_header)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

static int packet_crc()
{
    int err = EXIT_SUCCESS;
    uint8_t buffer[64];
    do {
        pUASDKpacket_header_t header = (pUASDKpacket_header_t)buffer;
        uint8_t* payload = (uint8_t*)(header + 1);
        memcpy(header, &test_header1, sizeof(UASDKpacket_header_t));
        for (int i = 0; i < (int)header->pll; i++)
        {
            payload[i] = (uint8_t)rand();
        }
        UASDKpacket_setedc((pUASDKpacket_t)header);
        if (EXIT_SUCCESS != (err = UASDKpacket_checkedc((pcUASDKpacket_t)header)))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
        }
        payload[18] ^= 0x20;
        if (EXIT_SUCCESS == UASDKpacket_checkedc((pcUASDKpacket_t)header))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }       
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

// Step 1: init counters
// Step 2: countup
// Step 3: confirm the values
// Step 4: reset
// Step 5: confirm the values
static int packet_counters()
{
    static const uint16_t count_targets[] = { 0x04ff, 0x8000, 0x0010, 1023 };
    static const uint8_t rads[] = { 0x01, 0x55, 0xcf, 0x04 };
    static const uint8_t sads[] = { 0x02, 0xcc, 0xfd, 0x80 };
    UASDKpacket_id_counter_t counters[4];
    uint16_t counter_count = (uint16_t)ARRAYSIZE(counters);
    int err = EXIT_SUCCESS;
    do {
        UASDKpacket_id_counter_clear(counters, counter_count);
        for (uint16_t i = 0; i < counter_count; i++)
        {
            if (EXIT_SUCCESS != (err = UASDKpacket_id_counter_new(counters, counter_count, rads[i], sads[i])))
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, err);
            }            
        }
        if (err) break;
        for (uint16_t i = 0; i < counter_count; i++)
        {
            for (uint16_t j = 0; j < count_targets[i]; j++)
            {
                UASDKpacket_id_counter_inc(counters, counter_count, rads[i], sads[i]);
            }
            uint16_t count = UASDKpacket_id_counter(counters, counter_count, rads[i], sads[i]);
            if (count != count_targets[i])
            {
                UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
            }
        }
        UASDKpacket_id_counter_reset(counters, counter_count, rads[0], sads[0]);
        if (0 != UASDKpacket_id_counter(counters, counter_count, rads[0], sads[0]))
        {
            UT_SHOWBREAK(stderr, __FUNCTION__, __LINE__, (err = EXIT_FAILURE));
        }
    } while (0);
    UT_SHOW(stderr, __FUNCTION__, __LINE__, err);
    return err;
}

int packet()
{
    int err = EXIT_SUCCESS;
    int err_each = EXIT_SUCCESS;
    do {
        err |= (err_each = packet_header_crc());
        if (err_each)
        {
            UT_SHOW(stderr, __FUNCTION__, __LINE__, err_each);
        }
        err |= (err_each = packet_crc());
        if (err_each)
        {
            UT_SHOW(stderr, __FUNCTION__, __LINE__, err_each);
        }
        err |= (err_each = packet_counters());
        {
            UT_SHOW(stderr, __FUNCTION__, __LINE__, err_each);
        }
    } while (0);
    return err;
}