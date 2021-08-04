#ifndef SOURCE_H_
#define SOURCE_H_
#define AVERAGING_FACTOR    16

typedef struct {
    int signal_register[AVERAGING_FACTOR];
} AveragedRand_t, *pAveragedRand_t;
typedef const AveragedRand_t *pcAveragedRand_t;

/*!
\brief get the scaled average of ar.
*/
int AveragedRand_get(pcAveragedRand_t ar);

typedef struct {
    float w; // angular frequency
    float dw; // phase per Tsample
} SineWaveSource_t, *pSineWaveSource_t;
typedef const SineWaveSource_t *pcSineWaveSource_t;

/*!
\brief get phase in radian and increment sample_count.
*/
float SineWaveSource_phase(pSineWaveSource_t ss);

/*!
\brief init SinWaveSource_t object
\param Tsample [in] sampling period
\param f [in] frequency
\return SineWaveSource_t object
*/
SinWaveSource_t SineWaveSource_create(float Tsample, float f);

typedef struct {
    float sine;
    float cosine;
    float random;
    uint32_t index;
} SourceSample_t, *pSourceSample_t;
typedef const SourceSample_t *pcSourceSample_t;

/*!
\brief update the latest sample
\param sourceSample [out]
*/
void SourceContext_update(pSourceSample_t sourceSample);

/*!
\brief reset the global source context
\param Tsample [in] sampling period
\param f [in] frequency of the sine wave generator
*/
void SourceContext_reset(float Tsample, float f);
#endif /* SOURCE_H_ */