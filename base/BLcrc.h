#ifndef BLCRC_H_
#define BLCRC_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

// CCITT CRC16 standard polynomial
// xxx_BE : big-endian (historical original)
// xxx_LE : little-endian (modern architecture migration)
#define CRC_CCITT_BE    ((uint16_t)0b0001000000100001) /* bit0: input, bit15: output */
#define CRC_CCITT_LE    ((uint16_t)0b1000010000001000) /* bit15: input, bit0: output */
#define CRC_INBIT_LE    ((uint16_t)0b1000000000000000) /* bit15=1, bit14-0: = 0*/

    typedef union {
        uint32_t u32;
        uint16_t u16[2];
        uint8_t u8[4];
    } BLcrc_shifter_t, *pBLcrc_shifter_t;

    typedef struct {
        BLcrc_shifter_t crc;
        uint16_t gen; // generator polynomial
    } BLcrc_t, *pBLcrc_t;

    typedef const BLcrc_t *pcBLcrc_t;

    /*!
    \brief Initialize CRC instance
    \param gen [in] generator polynomial
    \param seed [in] initial value of crc calculator
    */
    void BLcrc_init(pBLcrc_t crc, uint16_t gen, uint16_t seed);

    /*!
    \brief Calculate CRC with a byte
    \param px [in] a byte data
    */
    void BLcrc_put8(pBLcrc_t crc, const uint8_t* px);

    /*!
    \brief Calculate CRC with two bytes
    \param px [in] two bytes data
    */
    void BLcrc_put16(pBLcrc_t crc, const uint16_t* px);

    /*!
    \brief Calculate CRC with N bytes
    \param px [in] N bytes data
    \param N [in] byte count of input data
    */
    void BLcrc_puts(pBLcrc_t crc, const uint8_t* px, uint32_t N);
#ifdef __cplusplus
}
#endif
#endif /* BLCRC_H_ */
