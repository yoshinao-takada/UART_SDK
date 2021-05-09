#include    "sdk/UASDKpkt_testgen.h"
#include    <memory.h>
#define MAX_PAYLOAD_SIZE (DEFBUFSIZE - sizeof(UASDKpkt_t))

void UASDKpkt_testgen_init(pUASDKpkt_testgen_t pkt)
{
    int err = UASDKunibuf_initbyte(&pkt->buf, DEFBUFSIZE);
    if (err) abort();
    pkt->pkt = (pUASDKpkt_t)pkt->buf.bytebuf.buf;
}

void UASDKpkt_testgen_destroy(pUASDKpkt_testgen_t pkt)
{
    UASDKunibuf_destroy(&pkt->buf);
}

void UASDKpkt_testgen_setaddresses(pUASDKpkt_testgen_t pkt, uint8_t rad, uint8_t sad)
{
    pkt->pkt->rad = rad;
    pkt->pkt->sad = sad;
}

static void set_size_and_CRC(pUASDKpkt_t pkt, uint16_t size)
{
    pkt->opt.pll = size;
    UASDKpkt_setedc(pkt);
}

void UASDKpkt_testgen_setpayload(pUASDKpkt_testgen_t pkt, uint16_t payload_size, const void* payload_data)
{
    if (payload_size > MAX_PAYLOAD_SIZE)
    {
        abort();
    }
    pkt->pkt->opt.pll = payload_size;
    memcpy(pkt->pkt->payload, payload_data, (size_t)payload_size);
    pkt->buf.bytebuf.filled_bytes = (size_t)payload_size;
}

void UASDKpkt_testgen_setpayload_byteseq(pUASDKpkt_testgen_t pkt, uint16_t payload_size, uint8_t const_byte)
{
    if (payload_size > MAX_PAYLOAD_SIZE)
    {
        abort();
    }
    pkt->pkt->opt.pll = payload_size;
    memset(pkt->pkt->payload, (int)const_byte, (size_t)payload_size);
    pkt->buf.bytebuf.filled_bytes = (size_t)payload_size;
}

void UASDKpkt_testgen_setpayload_random(pUASDKpkt_testgen_t pkt, uint16_t payload_size)
{
    if (payload_size > MAX_PAYLOAD_SIZE)
    {
        abort();
    }
    pkt->pkt->opt.pll = payload_size;
    for (uint16_t i = 0; i < payload_size; i++)
    {
        pkt->pkt->payload[i] = (uint8_t)rand();
    }
    pkt->buf.bytebuf.filled_bytes = (size_t)payload_size;
}

void UASDKpkt_testgen_setshortdata(pUASDKpkt_testgen_t pkt, int16_t short_data)
{
    short_data |= 0x8000;
    pkt->pkt->opt.pll_sign = short_data;
    UASDKpkt_setedc(pkt->pkt);
}

#pragma region UASDKpkt_testgen
void UASDKpkt_testgen(pUASDKpkt_testgen_t pkt, pcUASDKpkt_gensrc_t gensrc)
{
    UASDKpkt_testgen_setaddresses(pkt, gensrc->rad, gensrc->sad);
    switch (gensrc->gentype)
    {
        case UASDKpkt_gentype_data:
        {
            UASDKpkt_testgen_setpayload(pkt, gensrc->opt.payload_size, gensrc->payload_src.payload_data);
            break;
        }
        case UASDKpkt_gentype_constbyte:
        {
            UASDKpkt_testgen_setpayload_byteseq(pkt, gensrc->opt.payload_size, gensrc->payload_src.const_byte);
            break;
        }
        case UASDKpkt_gentype_random:
        {
            UASDKpkt_testgen_setpayload_random(pkt, gensrc->opt.payload_size);
            break;
        }
        case UASDKpkt_gentype_short:
        {
            UASDKpkt_testgen_setshortdata(pkt, gensrc->opt.short_data);
            break;
        }
        default:
        {
            abort();
        }
    }
    UASDKpkt_setedc(pkt->pkt);
}
#pragma endregion UASDKpkt_testgen
