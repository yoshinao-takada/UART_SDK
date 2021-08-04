#ifndef BLAUTOBUF_H_
#define BLAUTOBUF_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    size_t bufsize;
    void* buf;
} BLautobuf_t, *pBLautobuf_t;
typedef const BLautobuf_t *pcBLautobuf_t;

/*!
\brief realloc buf->buf with newsize.
\return buf->buf
*/
void* BLautobuf_realloc(pBLautobuf_t buf, size_t newsize);

/*!
\brief free buf->buf
*/
void BLautobuf_free(pBLautobuf_t buf);

#ifdef __cplusplus
}
#endif
#endif /* BLAUTOBUF_H_ */