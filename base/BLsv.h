#ifndef BLSV_H_
#define BLSV_H_
#ifdef __cplusplus
#include    <ccomplex>
#include    <cstdint>
extern "C" {
#else
#include    <complex.h>
#include    <stdint.h>
#endif
#include    "base/BLbase.h"
#pragma region define_BL_basetypes
typedef int8_t      BL1i8_t;
typedef int16_t     BL1i16_t;
typedef int32_t     BL1i32_t;
typedef int64_t     BL1i64_t;
typedef uint8_t      BL1u8_t;
typedef uint16_t     BL1u16_t;
typedef uint32_t     BL1u32_t;
typedef uint64_t     BL1u64_t;
typedef float       BL1r32_t;
typedef double      BL1r64_t;
typedef long double BL1r128_t;
typedef float _Complex  BL1c64_t;
typedef double _Complex BL1c128_t;
typedef long double _Complex    BL1c256_t;
typedef intptr_t    BL1iptr_t;
typedef uintptr_t   BL1uptr_t;
#pragma endregion define_BL_basetypes
#pragma region define_BL_2D_shortvector_types
typedef int8_t      BL2i8_t[2];
typedef int16_t     BL2i16_t[2];
typedef int32_t     BL2i32_t[2];
typedef int64_t     BL2i64_t[2];
typedef uint8_t      BL2u8_t[2];
typedef uint16_t     BL2u16_t[2];
typedef uint32_t     BL2u32_t[2];
typedef uint64_t     BL2u64_t[2];
typedef float       BL2r32_t[2];
typedef double      BL2r64_t[2];
typedef long double BL2r128_t[2];
typedef float _Complex  BL2c64_t[2];
typedef double _Complex BL2c128_t[2];
typedef long double _Complex    BL2c256_t[2];
typedef intptr_t    BL2iptr_t[2];
typedef uintptr_t   BL2uptr_t[2];
#pragma endregion define_BL_2D_shortvector_types
#pragma region define_BL_3D_shortvector_types
typedef int8_t      BL3i8_t[3];
typedef int16_t     BL3i16_t[3];
typedef int32_t     BL3i32_t[3];
typedef int64_t     BL3i64_t[3];
typedef uint8_t      BL3u8_t[3];
typedef uint16_t     BL3u16_t[3];
typedef uint32_t     BL3u32_t[3];
typedef uint64_t     BL3u64_t[3];
typedef float       BL3r32_t[3];
typedef double      BL3r64_t[3];
typedef long double BL3r128_t[3];
typedef float _Complex  BL3c64_t[3];
typedef double _Complex BL3c128_t[3];
typedef long double _Complex    BL3c256_t[3];
typedef intptr_t    BL3iptr_t[3];
typedef uintptr_t   BL3uptr_t[3];
#pragma endregion define_BL_3D_shortvector_types
#pragma region define_BL_4D_shortvector_types
typedef int8_t      BL4i8_t[4];
typedef int16_t     BL4i16_t[4];
typedef int32_t     BL4i32_t[4];
typedef int64_t     BL4i64_t[4];
typedef uint8_t      BL4u8_t[4];
typedef uint16_t     BL4u16_t[4];
typedef uint32_t     BL4u32_t[4];
typedef uint64_t     BL4u64_t[4];
typedef float       BL4r32_t[4];
typedef double      BL4r64_t[4];
typedef long double BL4r128_t[4];
typedef float _Complex  BL4c64_t[4];
typedef double _Complex BL4c128_t[4];
typedef long double _Complex    BL4c256_t[4];
typedef intptr_t    BL4iptr_t[4];
typedef uintptr_t   BL4uptr_t[4];
#pragma endregion define_BL_4D_shortvector_types
#pragma region define_BL_6D_shortvector_types
typedef int8_t      BL6i8_t[6];
typedef int16_t     BL6i16_t[6];
typedef int32_t     BL6i32_t[6];
typedef int64_t     BL6i64_t[6];
typedef uint8_t      BL6u8_t[6];
typedef uint16_t     BL6u16_t[6];
typedef uint32_t     BL6u32_t[6];
typedef uint64_t     BL6u64_t[6];
typedef float       BL6r32_t[6];
typedef double      BL6r64_t[6];
typedef long double BL6r128_t[6];
typedef float _Complex  BL6c64_t[6];
typedef double _Complex BL6c128_t[6];
typedef long double _Complex    BL6c256_t[6];
typedef intptr_t    BL6iptr_t[6];
typedef uintptr_t   BL6uptr_t[6];
#pragma endregion define_BL_6D_shortvector_types
#pragma region define_BL_8D_shortvector_types
typedef int8_t      BL8i8_t[8];
typedef int16_t     BL8i16_t[8];
typedef int32_t     BL8i32_t[8];
typedef int64_t     BL8i64_t[8];
typedef uint8_t      BL8u8_t[8];
typedef uint16_t     BL8u16_t[8];
typedef uint32_t     BL8u32_t[8];
typedef uint64_t     BL8u64_t[8];
typedef float       BL8r32_t[8];
typedef double      BL8r64_t[8];
typedef long double BL8r128_t[8];
typedef float _Complex  BL8c64_t[8];
typedef double _Complex BL8c128_t[8];
typedef long double _Complex    BL8c256_t[8];
typedef intptr_t    BL8iptr_t[8];
typedef uintptr_t   BL8uptr_t[8];
#pragma endregion define_BL_8D_shortvector_types
#pragma region define_BL_9D_shortvector_types
typedef int8_t      BL9i8_t[9];
typedef int16_t     BL9i16_t[9];
typedef int32_t     BL9i32_t[9];
typedef int64_t     BL9i64_t[9];
typedef uint8_t      BL9u8_t[9];
typedef uint16_t     BL9u16_t[9];
typedef uint32_t     BL9u32_t[9];
typedef uint64_t     BL9u64_t[9];
typedef float       BL9r32_t[9];
typedef double      BL9r64_t[9];
typedef long double BL9r128_t[9];
typedef float _Complex  BL9c64_t[9];
typedef double _Complex BL9c128_t[9];
typedef long double _Complex    BL9c256_t[9];
typedef intptr_t    BL9iptr_t[9];
typedef uintptr_t   BL9uptr_t[9];
#pragma endregion define_BL_9D_shortvector_types
#pragma region define_BL_12D_shortvector_types
typedef int8_t      BL12i8_t[12];
typedef int16_t     BL12i16_t[12];
typedef int32_t     BL12i32_t[12];
typedef int64_t     BL12i64_t[12];
typedef uint8_t      BL12u8_t[12];
typedef uint16_t     BL12u16_t[12];
typedef uint32_t     BL12u32_t[12];
typedef uint64_t     BL12u64_t[12];
typedef float       BL12r32_t[12];
typedef double      BL12r64_t[12];
typedef long double BL12r128_t[12];
typedef float _Complex  BL12c64_t[12];
typedef double _Complex BL12c128_t[12];
typedef long double _Complex    BL12c256_t[12];
typedef intptr_t    BL12iptr_t[12];
typedef uintptr_t   BL12uptr_t[12];
#pragma endregion define_BL_12D_shortvector_types
#pragma region define_BL_16D_shortvector_types
typedef int8_t      BL16i8_t[16];
typedef int16_t     BL16i16_t[16];
typedef int32_t     BL16i32_t[16];
typedef int64_t     BL16i64_t[16];
typedef uint8_t      BL16u8_t[16];
typedef uint16_t     BL16u16_t[16];
typedef uint32_t     BL16u32_t[16];
typedef uint64_t     BL16u64_t[16];
typedef float       BL16r32_t[16];
typedef double      BL16r64_t[16];
typedef long double BL16r128_t[16];
typedef float _Complex  BL16c64_t[16];
typedef double _Complex BL16c128_t[16];
typedef long double _Complex    BL16c256_t[16];
typedef intptr_t    BL16iptr_t[16];
typedef uintptr_t   BL16uptr_t[16];
#pragma endregion define_BL_16D_shortvector_types

#define BLcopy2(dst,src)    (dst)[0] = (src)[0]; (dst)[1] = (src)[1];
#define BLcopy3(dst,src)    BLcopy2(dst,src); (dst)[2] = (src)[2];
#define BLcopy4(dst,src)    BLcopy3(dst,src); (dst)[3] = (src)[3];
#define BLcopy5(dst,src)    BLcopy4(dst,src); (dst)[4] = (src)[4];
#define BLcopy6(dst,src)    BLcopy5(dst,src); (dst)[5] = (src)[5];
#define BLcopy7(dst,src)    BLcopy6(dst,src); (dst)[6] = (src)[6];
#define BLcopy8(dst,src)    BLcopy7(dst,src); (dst)[7] = (src)[7];
#define BLcopy9(dst,src)    BLcopy8(dst,src); (dst)[8] = (src)[8];
#define BLcopy10(dst,src)    BLcopy9(dst,src); (dst)[9] = (src)[9];
#define BLcopy11(dst,src)    BLcopy10(dst,src); (dst)[10] = (src)[10];
#define BLcopy12(dst,src)    BLcopy11(dst,src); (dst)[11] = (src)[11];
#define BLcopy13(dst,src)    BLcopy12(dst,src); (dst)[12] = (src)[12];
#define BLcopy14(dst,src)    BLcopy13(dst,src); (dst)[13] = (src)[13];
#define BLcopy15(dst,src)    BLcopy14(dst,src); (dst)[14] = (src)[14];
#define BLcopy16(dst,src)    BLcopy15(dst,src); (dst)[15] = (src)[15];
#define BLcopyarray(dst,src,count)  for (int_fast8_t i = 0; i != count; i++) { dst[i] = src[i]; }

#define BLaddi2(dst,src)    (dst)[0] += (src)[0]; (dst)[1] += (src)[1];
#define BLaddi3(dst,src)    BLaddi2(dst,src); (dst)[2] += (src)[2];
#define BLaddi4(dst,src)    BLaddi3(dst,src); (dst)[3] += (src)[3];
#define BLaddi5(dst,src)    BLaddi4(dst,src); (dst)[4] += (src)[4];
#define BLaddi6(dst,src)    BLaddi5(dst,src); (dst)[5] += (src)[5];
#define BLaddi7(dst,src)    BLaddi6(dst,src); (dst)[6] += (src)[6];
#define BLaddi8(dst,src)    BLaddi7(dst,src); (dst)[7] += (src)[7];
#define BLaddi9(dst,src)    BLaddi8(dst,src); (dst)[8] += (src)[8];
#define BLaddi10(dst,src)    BLaddi9(dst,src); (dst)[9] += (src)[9];
#define BLaddi11(dst,src)    BLaddi10(dst,src); (dst)[10] += (src)[10];
#define BLaddi12(dst,src)    BLaddi11(dst,src); (dst)[11] += (src)[11];
#define BLaddi13(dst,src)    BLaddi12(dst,src); (dst)[12] += (src)[12];
#define BLaddi14(dst,src)    BLaddi13(dst,src); (dst)[13] += (src)[13];
#define BLaddi15(dst,src)    BLaddi14(dst,src); (dst)[14] += (src)[14];
#define BLaddi16(dst,src)    BLaddi15(dst,src); (dst)[15] += (src)[15];
#define BLaddiarray(dst,src,count)  for (int_fast8_t i = 0; i != count; i++) { dst[i] += src[i]; }

#define BLadd2(dst,src0,src1)    (dst)[0] = (src0)[0] + (src1)[0];(dst)[1] = (src0)[1] + (src1)[1];
#define BLadd3(dst,src0,src1)    BLadd2(dst,src0,src1);(dst)[2] = (src0)[2] + (src1)[2];
#define BLadd4(dst,src0,src1)    BLadd3(dst,src0,src1);(dst)[3] = (src0)[3] + (src1)[3];
#define BLadd5(dst,src0,src1)    BLadd4(dst,src0,src1);(dst)[4] = (src0)[4] + (src1)[4];
#define BLadd6(dst,src0,src1)    BLadd5(dst,src0,src1);(dst)[5] = (src0)[5] + (src1)[5];
#define BLadd7(dst,src0,src1)    BLadd6(dst,src0,src1);(dst)[6] = (src0)[6] + (src1)[6];
#define BLadd8(dst,src0,src1)    BLadd7(dst,src0,src1);(dst)[7] = (src0)[7] + (src1)[7];
#define BLadd9(dst,src0,src1)    BLadd8(dst,src0,src1);(dst)[8] = (src0)[8] + (src1)[8];
#define BLadd10(dst,src0,src1)    BLadd9(dst,src0,src1);(dst)[9] = (src0)[9] + (src1)[9];
#define BLadd11(dst,src0,src1)    BLadd10(dst,src0,src1);(dst)[10] = (src0)[10] + (src1)[10];
#define BLadd12(dst,src0,src1)    BLadd11(dst,src0,src1);(dst)[11] = (src0)[11] + (src1)[11];
#define BLadd13(dst,src0,src1)    BLadd12(dst,src0,src1);(dst)[12] = (src0)[12] + (src1)[12];
#define BLadd14(dst,src0,src1)    BLadd13(dst,src0,src1);(dst)[13] = (src0)[13] + (src1)[13];
#define BLadd15(dst,src0,src1)    BLadd14(dst,src0,src1);(dst)[14] = (src0)[14] + (src1)[14];
#define BLadd16(dst,src0,src1)    BLadd15(dst,src0,src1);(dst)[15] = (src0)[15] + (src1)[15];

#define BLsubi2(dst,src)    (dst)[0] -= (src)[0]; (dst)[1] -= (src)[1];
#define BLsubi3(dst,src)    BLsubi2(dst,src); (dst)[2] -= (src)[2];
#define BLsubi4(dst,src)    BLsubi3(dst,src); (dst)[3] -= (src)[3];
#define BLsubi5(dst,src)    BLsubi4(dst,src); (dst)[4] -= (src)[4];
#define BLsubi6(dst,src)    BLsubi5(dst,src); (dst)[5] -= (src)[5];
#define BLsubi7(dst,src)    BLsubi6(dst,src); (dst)[6] -= (src)[6];
#define BLsubi8(dst,src)    BLsubi7(dst,src); (dst)[7] -= (src)[7];
#define BLsubi9(dst,src)    BLsubi8(dst,src); (dst)[8] -= (src)[8];
#define BLsubi10(dst,src)    BLsubi9(dst,src); (dst)[9] -= (src)[9];
#define BLsubi11(dst,src)    BLsubi10(dst,src); (dst)[10] -= (src)[10];
#define BLsubi12(dst,src)    BLsubi11(dst,src); (dst)[11] -= (src)[11];
#define BLsubi13(dst,src)    BLsubi12(dst,src); (dst)[12] -= (src)[12];
#define BLsubi14(dst,src)    BLsubi13(dst,src); (dst)[13] -= (src)[13];
#define BLsubi15(dst,src)    BLsubi14(dst,src); (dst)[14] -= (src)[14];
#define BLsubi16(dst,src)    BLsubi15(dst,src); (dst)[15] -= (src)[15];
#define BLsubiarray(dst,src,count)  for (int_fast8_t i = 0; i != count; i++) { dst[i] -= src[i]; }

#define BLsub2(dst,src0,src1)    (dst)[0] = (src0)[0] - (src1)[0];(dst)[1] = (src0)[1] - (src1)[1];
#define BLsub3(dst,src0,src1)    BLsub2(dst,src0,src1);(dst)[2] = (src0)[2] - (src1)[2];
#define BLsub4(dst,src0,src1)    BLsub3(dst,src0,src1);(dst)[3] = (src0)[3] - (src1)[3];
#define BLsub5(dst,src0,src1)    BLsub4(dst,src0,src1);(dst)[4] = (src0)[4] - (src1)[4];
#define BLsub6(dst,src0,src1)    BLsub5(dst,src0,src1);(dst)[5] = (src0)[5] - (src1)[5];
#define BLsub7(dst,src0,src1)    BLsub6(dst,src0,src1);(dst)[6] = (src0)[6] - (src1)[6];
#define BLsub8(dst,src0,src1)    BLsub7(dst,src0,src1);(dst)[7] = (src0)[7] - (src1)[7];
#define BLsub9(dst,src0,src1)    BLsub8(dst,src0,src1);(dst)[8] = (src0)[8] - (src1)[8];
#define BLsub10(dst,src0,src1)    BLsub9(dst,src0,src1);(dst)[9] = (src0)[9] - (src1)[9];
#define BLsub11(dst,src0,src1)    BLsub10(dst,src0,src1);(dst)[10] = (src0)[10] - (src1)[10];
#define BLsub12(dst,src0,src1)    BLsub11(dst,src0,src1);(dst)[11] = (src0)[11] - (src1)[11];
#define BLsub13(dst,src0,src1)    BLsub12(dst,src0,src1);(dst)[12] = (src0)[12] - (src1)[12];
#define BLsub14(dst,src0,src1)    BLsub13(dst,src0,src1);(dst)[13] = (src0)[13] - (src1)[13];
#define BLsub15(dst,src0,src1)    BLsub14(dst,src0,src1);(dst)[14] = (src0)[14] - (src1)[14];
#define BLsub16(dst,src0,src1)    BLsub15(dst,src0,src1);(dst)[15] = (src0)[15] - (src1)[15];

#define BLmuli2(dst,src)    (dst)[0] *= (src)[0]; (dst)[1] *= (src)[1];
#define BLmuli3(dst,src)    BLmuli2(dst,src); (dst)[2] *= (src)[2];
#define BLmuli4(dst,src)    BLmuli3(dst,src); (dst)[3] *= (src)[3];
#define BLmuli5(dst,src)    BLmuli4(dst,src); (dst)[4] *= (src)[4];
#define BLmuli6(dst,src)    BLmuli5(dst,src); (dst)[5] *= (src)[5];
#define BLmuli7(dst,src)    BLmuli6(dst,src); (dst)[6] *= (src)[6];
#define BLmuli8(dst,src)    BLmuli7(dst,src); (dst)[7] *= (src)[7];
#define BLmuli9(dst,src)    BLmuli8(dst,src); (dst)[8] *= (src)[8];
#define BLmuli10(dst,src)    BLmuli9(dst,src); (dst)[9] *= (src)[9];
#define BLmuli11(dst,src)    BLmuli10(dst,src); (dst)[10] *= (src)[10];
#define BLmuli12(dst,src)    BLmuli11(dst,src); (dst)[11] *= (src)[11];
#define BLmuli13(dst,src)    BLmuli12(dst,src); (dst)[12] *= (src)[12];
#define BLmuli14(dst,src)    BLmuli13(dst,src); (dst)[13] *= (src)[13];
#define BLmuli15(dst,src)    BLmuli14(dst,src); (dst)[14] *= (src)[14];
#define BLmuli16(dst,src)    BLmuli15(dst,src); (dst)[15] *= (src)[15];
#define BLmuliarray(dst,src,count)  for (int_fast8_t i = 0; i != count; i++) { dst[i] *= src[i]; }

#define BLmul2(dst,src0,src1)    (dst)[0] = (src0)[0] * (src1)[0];(dst)[1] = (src0)[1] * (src1)[1];
#define BLmul3(dst,src0,src1)    BLmul2(dst,src0,src1);(dst)[2] = (src0)[2] * (src1)[2];
#define BLmul4(dst,src0,src1)    BLmul3(dst,src0,src1);(dst)[3] = (src0)[3] * (src1)[3];
#define BLmul5(dst,src0,src1)    BLmul4(dst,src0,src1);(dst)[4] = (src0)[4] * (src1)[4];
#define BLmul6(dst,src0,src1)    BLmul5(dst,src0,src1);(dst)[5] = (src0)[5] * (src1)[5];
#define BLmul7(dst,src0,src1)    BLmul6(dst,src0,src1);(dst)[6] = (src0)[6] * (src1)[6];
#define BLmul8(dst,src0,src1)    BLmul7(dst,src0,src1);(dst)[7] = (src0)[7] * (src1)[7];
#define BLmul9(dst,src0,src1)    BLmul8(dst,src0,src1);(dst)[8] = (src0)[8] * (src1)[8];
#define BLmul10(dst,src0,src1)    BLmul9(dst,src0,src1);(dst)[9] = (src0)[9] * (src1)[9];
#define BLmul11(dst,src0,src1)    BLmul10(dst,src0,src1);(dst)[10] = (src0)[10] * (src1)[10];
#define BLmul12(dst,src0,src1)    BLmul11(dst,src0,src1);(dst)[11] = (src0)[11] * (src1)[11];
#define BLmul13(dst,src0,src1)    BLmul12(dst,src0,src1);(dst)[12] = (src0)[12] * (src1)[12];
#define BLmul14(dst,src0,src1)    BLmul13(dst,src0,src1);(dst)[13] = (src0)[13] * (src1)[13];
#define BLmul15(dst,src0,src1)    BLmul14(dst,src0,src1);(dst)[14] = (src0)[14] * (src1)[14];
#define BLmul16(dst,src0,src1)    BLmul15(dst,src0,src1);(dst)[15] = (src0)[15] * (src1)[15];

#define BLdivi2(dst,src)    (dst)[0] /= (src)[0]; (dst)[1] /= (src)[1];
#define BLdivi3(dst,src)    BLdivi2(dst,src); (dst)[2] /= (src)[2];
#define BLdivi4(dst,src)    BLdivi3(dst,src); (dst)[3] /= (src)[3];
#define BLdivi5(dst,src)    BLdivi4(dst,src); (dst)[4] /= (src)[4];
#define BLdivi6(dst,src)    BLdivi5(dst,src); (dst)[5] /= (src)[5];
#define BLdivi7(dst,src)    BLdivi6(dst,src); (dst)[6] /= (src)[6];
#define BLdivi8(dst,src)    BLdivi7(dst,src); (dst)[7] /= (src)[7];
#define BLdivi9(dst,src)    BLdivi8(dst,src); (dst)[8] /= (src)[8];
#define BLdivi10(dst,src)    BLdivi9(dst,src); (dst)[9] /= (src)[9];
#define BLdivi11(dst,src)    BLdivi10(dst,src); (dst)[10] /= (src)[10];
#define BLdivi12(dst,src)    BLdivi11(dst,src); (dst)[11] /= (src)[11];
#define BLdivi13(dst,src)    BLdivi12(dst,src); (dst)[12] /= (src)[12];
#define BLdivi14(dst,src)    BLdivi13(dst,src); (dst)[13] /= (src)[13];
#define BLdivi15(dst,src)    BLdivi14(dst,src); (dst)[14] /= (src)[14];
#define BLdivi16(dst,src)    BLdivi15(dst,src); (dst)[15] /= (src)[15];
#define BLdiviarray(dst,src,count)  for (int_fast8_t i = 0; i != count; i++) { dst[i] /= src[i]; }

#define BLdiv2(dst,src0,src1)    (dst)[0] = (src0)[0] / (src1)[0];(dst)[1] = (src0)[1] / (src1)[1];
#define BLdiv3(dst,src0,src1)    BLdiv2(dst,src0,src1);(dst)[2] = (src0)[2] / (src1)[2];
#define BLdiv4(dst,src0,src1)    BLdiv3(dst,src0,src1);(dst)[3] = (src0)[3] / (src1)[3];
#define BLdiv5(dst,src0,src1)    BLdiv4(dst,src0,src1);(dst)[4] = (src0)[4] / (src1)[4];
#define BLdiv6(dst,src0,src1)    BLdiv5(dst,src0,src1);(dst)[5] = (src0)[5] / (src1)[5];
#define BLdiv7(dst,src0,src1)    BLdiv6(dst,src0,src1);(dst)[6] = (src0)[6] / (src1)[6];
#define BLdiv8(dst,src0,src1)    BLdiv7(dst,src0,src1);(dst)[7] = (src0)[7] / (src1)[7];
#define BLdiv9(dst,src0,src1)    BLdiv8(dst,src0,src1);(dst)[8] = (src0)[8] / (src1)[8];
#define BLdiv10(dst,src0,src1)    BLdiv9(dst,src0,src1);(dst)[9] = (src0)[9] / (src1)[9];
#define BLdiv11(dst,src0,src1)    BLdiv10(dst,src0,src1);(dst)[10] = (src0)[10] / (src1)[10];
#define BLdiv12(dst,src0,src1)    BLdiv11(dst,src0,src1);(dst)[11] = (src0)[11] / (src1)[11];
#define BLdiv13(dst,src0,src1)    BLdiv12(dst,src0,src1);(dst)[12] = (src0)[12] / (src1)[12];
#define BLdiv14(dst,src0,src1)    BLdiv13(dst,src0,src1);(dst)[13] = (src0)[13] / (src1)[13];
#define BLdiv15(dst,src0,src1)    BLdiv14(dst,src0,src1);(dst)[14] = (src0)[14] / (src1)[14];
#define BLdiv16(dst,src0,src1)    BLdiv15(dst,src0,src1);(dst)[15] = (src0)[15] / (src1)[15];

#define BLsli2(dst,src)    (dst)[0] <<= (src)[0]; (dst)[1] <<= (src)[1];
#define BLsli3(dst,src)    BLsli2(dst,src); (dst)[2] <<= (src)[2];
#define BLsli4(dst,src)    BLsli3(dst,src); (dst)[3] <<= (src)[3];
#define BLsli5(dst,src)    BLsli4(dst,src); (dst)[4] <<= (src)[4];
#define BLsli6(dst,src)    BLsli5(dst,src); (dst)[5] <<= (src)[5];
#define BLsli7(dst,src)    BLsli6(dst,src); (dst)[6] <<= (src)[6];
#define BLsli8(dst,src)    BLsli7(dst,src); (dst)[7] <<= (src)[7];
#define BLsli9(dst,src)    BLsli8(dst,src); (dst)[8] <<= (src)[8];
#define BLsli10(dst,src)    BLsli9(dst,src); (dst)[9] <<= (src)[9];
#define BLsli11(dst,src)    BLsli10(dst,src); (dst)[10] <<= (src)[10];
#define BLsli12(dst,src)    BLsli11(dst,src); (dst)[11] <<= (src)[11];
#define BLsli13(dst,src)    BLsli12(dst,src); (dst)[12] <<= (src)[12];
#define BLsli14(dst,src)    BLsli13(dst,src); (dst)[13] <<= (src)[13];
#define BLsli15(dst,src)    BLsli14(dst,src); (dst)[14] <<= (src)[14];
#define BLsli16(dst,src)    BLsli15(dst,src); (dst)[15] <<= (src)[15];
#define BLsliarray(dst,src,count)  for (int_fast8_t i = 0; i != count; i++) { dst[i] <<= src[i]; }

#define BLsl2(dst,src0,src1)    (dst)[0] = (src0)[0] << (src1)[0];(dst)[1] = (src0)[1] << (src1)[1];
#define BLsl3(dst,src0,src1)    BLsl2(dst,src0,src1);(dst)[2] = (src0)[2] << (src1)[2];
#define BLsl4(dst,src0,src1)    BLsl3(dst,src0,src1);(dst)[3] = (src0)[3] << (src1)[3];
#define BLsl5(dst,src0,src1)    BLsl4(dst,src0,src1);(dst)[4] = (src0)[4] << (src1)[4];
#define BLsl6(dst,src0,src1)    BLsl5(dst,src0,src1);(dst)[5] = (src0)[5] << (src1)[5];
#define BLsl7(dst,src0,src1)    BLsl6(dst,src0,src1);(dst)[6] = (src0)[6] << (src1)[6];
#define BLsl8(dst,src0,src1)    BLsl7(dst,src0,src1);(dst)[7] = (src0)[7] << (src1)[7];
#define BLsl9(dst,src0,src1)    BLsl8(dst,src0,src1);(dst)[8] = (src0)[8] << (src1)[8];
#define BLsl10(dst,src0,src1)    BLsl9(dst,src0,src1);(dst)[9] = (src0)[9] << (src1)[9];
#define BLsl11(dst,src0,src1)    BLsl10(dst,src0,src1);(dst)[10] = (src0)[10] << (src1)[10];
#define BLsl12(dst,src0,src1)    BLsl11(dst,src0,src1);(dst)[11] = (src0)[11] << (src1)[11];
#define BLsl13(dst,src0,src1)    BLsl12(dst,src0,src1);(dst)[12] = (src0)[12] << (src1)[12];
#define BLsl14(dst,src0,src1)    BLsl13(dst,src0,src1);(dst)[13] = (src0)[13] << (src1)[13];
#define BLsl15(dst,src0,src1)    BLsl14(dst,src0,src1);(dst)[14] = (src0)[14] << (src1)[14];
#define BLsl16(dst,src0,src1)    BLsl15(dst,src0,src1);(dst)[15] = (src0)[15] << (src1)[15];

#define BLsri2(dst,src)    (dst)[0] >>= (src)[0]; (dst)[1] >>= (src)[1];
#define BLsri3(dst,src)    BLsri2(dst,src); (dst)[2] >>= (src)[2];
#define BLsri4(dst,src)    BLsri3(dst,src); (dst)[3] >>= (src)[3];
#define BLsri5(dst,src)    BLsri4(dst,src); (dst)[4] >>= (src)[4];
#define BLsri6(dst,src)    BLsri5(dst,src); (dst)[5] >>= (src)[5];
#define BLsri7(dst,src)    BLsri6(dst,src); (dst)[6] >>= (src)[6];
#define BLsri8(dst,src)    BLsri7(dst,src); (dst)[7] >>= (src)[7];
#define BLsri9(dst,src)    BLsri8(dst,src); (dst)[8] >>= (src)[8];
#define BLsri10(dst,src)    BLsri9(dst,src); (dst)[9] >>= (src)[9];
#define BLsri11(dst,src)    BLsri10(dst,src); (dst)[10] >>= (src)[10];
#define BLsri12(dst,src)    BLsri11(dst,src); (dst)[11] >>= (src)[11];
#define BLsri13(dst,src)    BLsri12(dst,src); (dst)[12] >>= (src)[12];
#define BLsri14(dst,src)    BLsri13(dst,src); (dst)[13] >>= (src)[13];
#define BLsri15(dst,src)    BLsri14(dst,src); (dst)[14] >>= (src)[14];
#define BLsri16(dst,src)    BLsri15(dst,src); (dst)[15] >>= (src)[15];
#define BLsriarray(dst,src,count)  for (int_fast8_t i = 0; i != count; i++) { dst[i] >>= src[i]; }

#define BLsr2(dst,src0,src1)    (dst)[0] = (src0)[0] >> (src1)[0];(dst)[1] = (src0)[1] >> (src1)[1];
#define BLsr3(dst,src0,src1)    BLsr2(dst,src0,src1);(dst)[2] = (src0)[2] >> (src1)[2];
#define BLsr4(dst,src0,src1)    BLsr3(dst,src0,src1);(dst)[3] = (src0)[3] >> (src1)[3];
#define BLsr5(dst,src0,src1)    BLsr4(dst,src0,src1);(dst)[4] = (src0)[4] >> (src1)[4];
#define BLsr6(dst,src0,src1)    BLsr5(dst,src0,src1);(dst)[5] = (src0)[5] >> (src1)[5];
#define BLsr7(dst,src0,src1)    BLsr6(dst,src0,src1);(dst)[6] = (src0)[6] >> (src1)[6];
#define BLsr8(dst,src0,src1)    BLsr7(dst,src0,src1);(dst)[7] = (src0)[7] >> (src1)[7];
#define BLsr9(dst,src0,src1)    BLsr8(dst,src0,src1);(dst)[8] = (src0)[8] >> (src1)[8];
#define BLsr10(dst,src0,src1)    BLsr9(dst,src0,src1);(dst)[9] = (src0)[9] >> (src1)[9];
#define BLsr11(dst,src0,src1)    BLsr10(dst,src0,src1);(dst)[10] = (src0)[10] >> (src1)[10];
#define BLsr12(dst,src0,src1)    BLsr11(dst,src0,src1);(dst)[11] = (src0)[11] >> (src1)[11];
#define BLsr13(dst,src0,src1)    BLsr12(dst,src0,src1);(dst)[12] = (src0)[12] >> (src1)[12];
#define BLsr14(dst,src0,src1)    BLsr13(dst,src0,src1);(dst)[13] = (src0)[13] >> (src1)[13];
#define BLsr15(dst,src0,src1)    BLsr14(dst,src0,src1);(dst)[14] = (src0)[14] >> (src1)[14];
#define BLsr16(dst,src0,src1)    BLsr15(dst,src0,src1);(dst)[15] = (src0)[15] >> (src1)[15];

#define BLprod2(src)        (src)[0]*(src)[1]
#define BLprod3(src)        BLprod2(src)*(src)[2]
#define BLprod4(src)        BLprod3(src)*(src)[3]
#define BLprod5(src)        BLprod4(src)*(src)[4]
#define BLprod6(src)        BLprod5(src)*(src)[5]
#define BLprod7(src)        BLprod6(src)*(src)[6]
#define BLprod8(src)        BLprod7(src)*(src)[7]

#define BLeq2(a0,a1)        (((a0)[0]==(a1)[0]) && ((a0)[1]==(a1)[1]))
#define BLeq3(a0,a1)        (((a0)[0]==(a1)[0]) && ((a0)[1]==(a1)[1]) && ((a0)[2]==(a1)[2]))
#define BLeq4(a0,a1)        (((a0)[0]==(a1)[0]) && ((a0)[1]==(a1)[1]) && ((a0)[2]==(a1)[2]) && ((a0)[3]==(a1)[3]))
#define BLeq5(a0,a1)        (((a0)[0]==(a1)[0]) && ((a0)[1]==(a1)[1]) && ((a0)[2]==(a1)[2]) && \
                            ((a0)[3]==(a1)[3]) && ((a0)[4]==(a1)[4]))
#define BLeq6(a0,a1)        (((a0)[0]==(a1)[0]) && ((a0)[1]==(a1)[1]) && ((a0)[2]==(a1)[2]) && \
                            ((a0)[3]==(a1)[3]) && ((a0)[4]==(a1)[4]) && ((a0)[5]==(a1)[5]))
#define BLeq7(a0,a1)        (((a0)[0]==(a1)[0]) && ((a0)[1]==(a1)[1]) && ((a0)[2]==(a1)[2]) && \
                            ((a0)[3]==(a1)[3]) && ((a0)[4]==(a1)[4]) && ((a0)[5]==(a1)[5]) && ((a0)[6]==(a1)[6]))
#define BLeq8(a0,a1)        (((a0)[0]==(a1)[0]) && ((a0)[1]==(a1)[1]) && ((a0)[2]==(a1)[2]) && \
                            ((a0)[3]==(a1)[3]) && ((a0)[4]==(a1)[4]) && ((a0)[5]==(a1)[5]) && \
                            ((a0)[6]==(a1)[6]) && ((a0)[7]==(a1)[7]))
#ifdef __cplusplus
}
#endif
#endif /* BLSV_H_ */
