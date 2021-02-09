#ifndef BLARRAY_H_
#define BLARRAY_H_
#include    "base/BLbase.h"
#include    "base/BLsv.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    BL4u32_t size; // { element_size, x_stride, y_stride, z_stride }
    BL1u8_t buffer[0];
} BLarray_t, *pBLarray_t;
typedef const BLarray_t *pcBLarray_t;

#define BLarray_alloc(ptr_var,_size_) \
    (ptr_var) = (pBLarray_t)malloc(sizeof(BLarray_t) + BLprod4(_size_)); \
    BLcopy4((ptr_var)->size, _size_);

#define BLarray_alloca(ptr_var,_size_) \
    (ptr_var) = (pBLarray_t)alloca(sizeof(BLarray_t) + BLprod4(_size_)); \
    BLcopy4((ptr_var)->size, _size_);

#define BLarray_begin(ptr_var)   ((ptr_var)->buffer)
#define BLarray_at(ptr_var,offset) (BLarray_begin(ptr_var) + \
    (offset[0] + (offset[1] + (offset[2] + (offset[3] * (ptr_var)->size[2]) * (ptr_var)->size[1]) * (ptr_var)->size[0]))

#define BLarray_at1D(ptr_var,offset) (BLarray_begin(ptr_var) + (ptr_var)->size[0] * (offset))

#define BLarray_at2D(ptr_var,offset) \
(BLarray_begin(ptr_var) + (ptr_var)->size[0] * ((offset)[0] + (offset)[1] * (ptr_var)->size[1]))

#define BLarray_at3D(ptr_var,offset) (BLarray_begin(ptr_var) + \
(ptr_var)->size[0] * ( \
    (offset)[0] + (offset)[1] * (ptr_var)->size[1] + (offset)[2] * (ptr_var)->size[1] * (ptr_var)->size[2] \
))

#define BLarray_size1D(T,Cx) { sizeof(T),Cx,1,1 }
#define BLarray_size2D(T,Cx,Cy) { sizeof(T),Cx,Cy,1 }
#define BLarray_size3D(T,Cx,Cy,Cz) { sizeof(T),Cx,Cy,Cz }

void BLarray_copy1D(
    pBLarray_t dst,
    BL1u32_t dst_offset,
    pcBLarray_t src,
    BL1u32_t src_offset,
    BL1u32_t block_size
);

void BLarray_copy2D(
    pBLarray_t dst,
    BL2u32_t dst_offset,
    pcBLarray_t src,
    BL2u32_t src_offset,
    BL2u32_t block_size
);

void BLarray_copy3D(
    pBLarray_t dst,
    BL3u32_t dst_offset,
    pcBLarray_t src,
    BL3u32_t src_offset,
    BL3u32_t block_size
);

void BLarray_fill1D(
    pBLarray_t dst,
    BL1u32_t dst_offset,
    BL1u32_t block_size,
    const void* filler
);

void BLarray_fill2D(
    pBLarray_t dst,
    BL2u32_t dst_offset,
    BL2u32_t block_size,
    const void* filler
);
#ifdef __cplusplus
}
#endif
#endif /* BLARRAY_H_ */