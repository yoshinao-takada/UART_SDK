#include "cui/BLautobuf.h"
#include <malloc.h>
void* BLautobuf_realloc(pBLautobuf_t buf, size_t newsize)
{
    buf->buf = realloc(buf->buf, newsize);
    if (NULL == buf->buf)
    {
        buf->bufsize = 0;
    }
    else
    {
        buf->bufsize = newsize;
    }
    return buf->buf;
}

void BLautobuf_free(pBLautobuf_t buf)
{
    if (buf->buf)
    {
        free(buf->buf);
        buf->buf = NULL;
        buf->bufsize = 0;
    }
}

