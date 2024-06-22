#include <WaveSabreCore/ExternalSample.h>

extern "C" int WaveSabre_UncompressSample(WaveSabreCore::UncompressedSample *sample);

namespace WaveSabreCore
{
	ExternalSample::ExternalSample(char *compressedData, int compressedSize, SampleFormat *format)
		: UncompressedSample(compressedData, compressedSize, (SampleFormatHeader*)format, format->Size)
	{
		WaveSabre_UncompressSample(this);
	}
}