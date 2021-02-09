#ifndef UASDK_SERIALIZE_H_
#define UASDK_SERIALIZE_H_
// including serializer definitions of available serializable data structures
#include "serialize/UASDK_string.h"
#include "serialize/UASDK_wstring.h"
#include "serialize/UASDK_i16array.h"
#include "serialize/UASDK_r32array.h"

/*! 
\file UASDK_serialize.h
\brief UASDK serializer framework
*/
#ifdef __cplusplus
extern "C" {
#endif

/*!
\brief signature declaration of and deserializer of UART_SDK framework
\param src [in] source data to serialize
\param dst [in,out] serialized destination data
\return result code
*/
typedef int (*UASDK_serializer_t)(const void* src, pBLarray_t dst);

/*!
\brief signature of byte count estimator of serialized data
\param src [in] source data
\return byte count of serialized data
*/
typedef uint32_t (*UASDK_serialized_byte_count_estimator_t)(const void* src);

/*!
\brief signature declaration of deserializer
\param src [in] byte array to deserialize
\param dst [out] deserialized data
\return result code
*/
typedef int (*UASDK_deserializer_t)(pcBLarray_t src, void* dst);

/*!
\brief signature of byte count estimator of desrialized data
\param src [in] source data to deserialize
\return byte count of deserialized data
*/
typedef uint32_t (*UASDK_deserialized_byte_count_estimator_t)(pcBLarray_t src);

typedef enum {
    ID8cc_UAstring,
    ID8cc_UAwstring,
    ID8cc_UAi16array,
    ID8cc_UAr32array,
} ID8cc_t;

typedef struct {
    ID8cc_t enum_tag;
    uint8_t code[8];
} ID8cc_code_t, *pID8cc_code_t;
typedef const ID8cc_code_t *pcID8cc_code_t;

#define ID8CC_CODES { \
    { ID8cc_UAstring, UASDK_string_8cc }, \
    { ID8cc_UAwstring, { 'U','A','W','S','T','R','I','N' } }, \
    { ID8cc_UAi16array, { 'U','A','I','1','6','A','R','Y' } }, \
    { ID8cc_UAr32array, { 'U','A','R','3','2','A','R','Y' } } \
}

typedef struct {
    uint32_t element_count;
    ID8cc_code_t element[0];
} ID8cc_codes_t, *pID8cc_codes_t;
typedef const ID8cc_codes_t *pcID8cc_codes_t;

/*!
\brief allocate a memory block using alloc(). memory block is in heap and user code must release it.
*/
int ID8cc_codes_new(uint32_t element_count, pID8cc_codes_t* pp);
/*!
\brief allocate a memory block using alloca(). memory block is on a current stack frame and released automatically.
*/
int ID8cc_codes_newa(uint32_t element_count, pID8cc_codes_t* pp);

/*!
\find
*/
pcID8cc_code_t ID8cc_codes_find(pcID8cc_codes_t codes, const uint8_t* code_cc8);

/*!
\return 1: equal, 0: NOT equal
*/
int cc8_equal(const uint8_t code0[8], const uint8_t code1[8]);

typedef struct {
    ID8cc_t id;
    uint8_t code[8];
    const char* description;
    UASDK_serializer_t serialize;
    UASDK_serializer_t deserialize;
} UASDK_SERIALIZE_item_t, *pUASDK_SERIALIZE_item_t;
typedef const UASDK_SERIALIZE_item_t *pcUASDK_SERIALIZE_item_t;

#ifdef __cplusplus
}
#endif
#endif /* UASDK_SERIALIZE_H_ */