#include    "SimDevice/source.h"
#include    <math.h>

int AveragedRand_get(pcAveragedRand_t ar)
{
    int i = 0;
    for (; i < (AVERAGING_FACTOR - 1); i++)
    {
        ar->signal_register[i] = ar->signal_register[i + 1];
    }
    ar->signal_register = rand();
    int scaled_average = 0;
    for (int i = 0; i < AVERAGING_FACTOR; i++)
    {
        scaled_average += ar->signal_register[i]/AVERAGING_FACTOR;
    }
    return scaled_average;
}

float SineWaveSource_phase(pSineWaveSource_t ss)
{
    ss->w += ss->dw;
    return ss->w;
}

SineWaveSource_t SineWaveSource_create(float Tsample, float f)
{
    SineWaveSource_t ss = { 0.0f, (float)(2.0 * M_PI * f * Tsample) };
    return ss;
}

typedef struct {
    AveragedRand_t ar;
    SineWaveSource_t sws;
    uint32_t index;
} SourceContext_t, *pSourceContext_t;
typedef const SourceContext_t *pcSourceContext_t;

static SourceContext_t context = { { 0 }, SineWaveSource_create(1.0e-3f, 60.0f), 0 };

void SourceContext_update(pSourceSample_t sourceSample)
{
    sourceSample->random = AveragedRand_get(&context.ar);
    float phase = SineWaveSource_phase(&context.sws);
    sourceSample->cosine = cosf(phase);
    sourceSample->sine = sinf(phase);
    sourceSample->index = context.index++;
    return sourceSample;
}

void SourceContext_reset(float Tsample, float f)
{
    for (int i = 0; i < AVERAGING_FACTOR; i++)
    {
        context.ar.signal_register[i] = 0;
    }
    context.sws = SineWaveSource_create(Tsample, f);
    context.index = 0;
}