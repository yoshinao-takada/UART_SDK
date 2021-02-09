#ifndef UASDK_STRING_H_
#define UASDK_STRING_H_
/*!
\file serialize/UASDK_string.h
\brief serializable string encoded by UTF-8
*/
#include    "base/BLarray1D.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef BLarray1D_t   UASDK_string_t, *pUASDK_string_t;
typedef const UASDK_string_t *pcUASDK_string_t;

/*!
\brief initializer using malloc
\param src_string [in] null terminated C style string
\param ppstring [out] array of uint8_t including the terminating null character
*/
int UASDK_string_new(const char* src_string, pUASDK_string_t* ppstring);

/*!
\brief initializer using malloc
\param src_char_count [in] source string length counted in bytes
\param src_string [in] source string which can contain null characters at any position
\param ppstring [out] array of uint8_t. it doest not have to have the terminating null character.
The string contents is just a copy of the source string.
*/
int UASDK_string_new2(uint32_t src_char_count, const char* src_string, pUASDK_string_t* ppstring);

/*!
\brief serializer
\param src [in] source data to serialize
\param dst [in,out] serialized destination data
\return result code
*/
int UASDK_string_serialize(const void* src, void* dst);

/*!
\brief byte count estimator of serialized data
\param src [in] source data
\return byte count of serialized data
*/
int UASDK_string_serialize_estimate(const void* src);

/*!
\brief 8CC identifier
*/
#define UASDK_string_8cc { 'U','A','S','T','R','I','N','G' }

/*!
\brief deserializer
\param src_byte_count [in] source buffer byte count which must >= actual data byte count
\param src [in] source data to deserialize
\param dst [out] deserialized string. *dst is filled with a serialized image of UASDK_string_t.
\return result code
*/
int UASDK_string_deserialize(uint16_t src_byte_count, const void* src, void* dst);

/*!
\brief byte count estimator of deserialized data
\param src_byte_count [in] source buffer byte count which must >= actual data byte count
\param src [in] source data to deserialize
\param src [in] source data
\return byte count of deserialized data
*/
int UASDK_string_deserialize_estimate(uint16_t src_byte_count, const void* src);
#ifdef __cplusplus
}
#endif
#endif /* UASDK_STRING_H_ */