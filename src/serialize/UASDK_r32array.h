#ifndef UASDK_I32ARRAY_H_
#define UASDK_I32ARRAY_H_
#include    "base/BLarray1D.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef BLarray1D_t     UASDK_r32array_t, *pUASDK_r32array_t;
typedef const UASDK_r32array_t  *pcUASDK_r32array_t;

/*!
\brief initializer using malloc
\param src_count [in] count of BL1r32_t elements in src_data
\param src_data [in] pointer to a memory block holding int16_t numbers
\param pparray [out] array of BL1r32_t.
*/
int UASDK_r32array_new(uint32_t src_count, const BL1r32_t* src_data, pUASDK_r32array_t* pparray);

/*!
\brief serializer
\param src [in] source data to serialize
\param dst [in,out] serialized destination data
\return result code
*/
int UASDK_r32array_serialize(const void* src, void* dst);

/*!
\brief byte count estimator of serialized data
\param src [in] source data
\return byte count of serialized data
*/
int UASDK_r32array_serialize_estimate(const void* src);

/*!
\brief 8CC identifier
*/
#define UASDK_r32array_8cc { 'U','A','R','3','2','A','R','Y' }

/*!
\brief deserializer
\param src_byte_count [in] source buffer byte count which must >= actual data byte count
\param src [in] source data to deserialize
\param dst [out] deserialized string. *dst is filled with a serialized image of UASDK_r32array_t.
\return result code
*/
int UASDK_r32array_deserialize(uint16_t src_byte_count, const void* src, void* dst);

/*!
\brief byte count estimator of deserialized data
\param src_byte_count [in] source buffer byte count which must >= actual data byte count
\param src [in] source data to deserialize
\param src [in] source data
\return byte count of deserialized data
*/
int UASDK_r32array_deserialize_estimate(uint16_t src_byte_count, const void* src);

#ifdef __cplusplus
}
#endif
#endif /* UASDK_I32ARRAY_H_ */