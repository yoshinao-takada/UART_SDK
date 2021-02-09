#include    "base/BLarray.h"
#include    <memory.h>

void BLarray_copy1D(
    pBLarray_t dst,
    BL1u32_t dst_offset,
    pcBLarray_t src,
    BL1u32_t src_offset,
    BL1u32_t block_size
) {
    size_t cb_copy_block = dst->size[0] * block_size;
    void* pv_dst = BLarray_at1D(dst, dst_offset);
    const void* pv_src = BLarray_at1D(src, src_offset);
    memcpy(pv_dst, pv_src, cb_copy_block);
}

void BLarray_copy2D(
    pBLarray_t dst,
    BL2u32_t dst_offset,
    pcBLarray_t src,
    BL2u32_t src_offset,
    BL2u32_t block_size
) {
    size_t cb_copy_block = dst->size[0] * block_size[0];
    uint32_t cb_dst_step = dst->size[0] * dst->size[1];
    uint32_t cb_src_step = src->size[0] * src->size[1];
    BL1u8_t* pu8_dst = BLarray_at2D(dst, dst_offset);
    const BL1u8_t* pu8_src = BLarray_at2D(src, src_offset);
    for (uint32_t iy = 0; iy != block_size[1]; iy++)
    {
        memcpy(pu8_dst + iy * cb_dst_step, pu8_src + iy * cb_src_step, cb_copy_block);
    }
}

void BLarray_copy3D(
    pBLarray_t dst,
    BL3u32_t dst_offset,
    pcBLarray_t src,
    BL3u32_t src_offset,
    BL3u32_t block_size
) {
    size_t cb_copy_block = dst->size[0] * block_size[0];
    uint32_t cb_dst_step = dst->size[0] * dst->size[1];
    uint32_t cb_src_step = src->size[0] * src->size[1];
    BL1u8_t* pu8_dst = BLarray_at3D(dst, dst_offset);
    const BL1u8_t* pu8_src = BLarray_at3D(src, src_offset);
    for (uint32_t iz = 0; iz != block_size[2]; iz++)
    {
        for (uint32_t iy = 0; iy != block_size[1]; iy++)
        {
            uint32_t dst_linear_offset = (iy + iz * dst->size[2]) * cb_dst_step;
            uint32_t src_linear_offset = (iy + iz * src->size[2]) * cb_src_step;
            memcpy(pu8_dst + dst_linear_offset, pu8_src + src_linear_offset, cb_copy_block);
        }
    }
}


void BLarray_fill1D(
    pBLarray_t dst,
    BL1u32_t dst_offset,
    BL1u32_t block_size,
    const void* filler
) {
    BL1u8_t* pu8_dst = BLarray_at1D(dst, dst_offset);
    for (BL1u32_t i = 0; i != block_size; i++)
    {
        memcpy(pu8_dst + i * block_size, filler, block_size);
    }
}

void BLarray_fill2D(
    pBLarray_t dst,
    BL2u32_t dst_offset,
    BL2u32_t block_size,
    const void* filler
) {
    BL1u8_t* pu8_dst = BLarray_at2D(dst, dst_offset);
    for (BL1u32_t iy = 0; iy != block_size[1]; iy++)
    {
        for (BL1u32_t ix = 0; ix != block_size[0]; ix++)
        {
            uint32_t linear_offset = dst->size[0] * (ix + iy * dst->size[1]);
            memcpy(pu8_dst + linear_offset, filler, dst->size[0]);
        }
    }
}
