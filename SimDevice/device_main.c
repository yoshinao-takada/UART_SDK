#include "base/BLdispatcher.h"
#include "SimDevice/source.h"
#include "base/BLringbuf.h"

static const struct timespec systick_period = BL1msTIMESPEC;
static const float Tsample = 1e-3f;
static const float fSineWave = 60.0f;
static pBLringbuf_t signal_samples = NULL;

void* PushSampleCallback(void* params)
{
	pBLringbuf_t rb = (pBLringbuf_t)params;
	SourceSample_t ss;
	SourceContext_update(&ss);
	PushSample(rb, &ss);
	return params;
}

/*!
\brief dispatcher initializer
[0]: signal source sampling service
[1]: issuing service requests to a host process
*/
static const BLdispatcher_core_t dispatchers[2] =
{
	{ 1, 1, PushSampleCallback, (void*)signal_samples },
	{ 16, 16, NULL, NULL }
};

void PushSample(pBLringbuf_t rb, pcSourceSample_t pcss)
{
	static const uint16_t pushSize = (uint16_t)sizeof(SourceSample_t);
	BLringbuf_putforce(rb, pushSize, (const uint8_t*)pcss);
}

int main()
{
	int err = EXIT_SUCCESS;
	do {
		if (EXIT_SUCCESS != (err = BLringbuf_new(10, &signal_samples)))
		{
			break;
		}
		SourceContext_reset(Tsample, fSineWave);

	} while (0);
	BLSAFEFREE(&signal_samples);
	return err;
}