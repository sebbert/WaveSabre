#include <WaveSabreCore/EmscriptenSample.h>
#include <WaveSabreCore/Specimen.h>

extern "C"
{
	EMSCRIPTEN_KEEPALIVE
	float* WaveSabre_FinishUncompressSample(WaveSabreCore::EmscriptenSample *sample, int sampleLength)
	{
		return sample->FinishUncompress(sampleLength);
	}
}

namespace WaveSabreCore
{
	EmscriptenSample::EmscriptenSample(char *compressedData, int compressedSize, SampleFormat *format, Specimen *specimen)
		: UncompressedSample(compressedData, compressedSize, (SampleFormatHeader*)format, format->Size)
		, SpecimenDevice(specimen)
	{
	}

	void EmscriptenSample::BeginUncompress()
	{
		WaveSabre_BeginUncompressSample(this, CompressedData, CompressedSize);
	}

	float* EmscriptenSample::FinishUncompress(int sampleLength)
	{
		UncompressedSize = sampleLength * sizeof(float);
		SampleLength = sampleLength;
		if (SampleData) delete[] SampleData;
		SampleData = new float[sampleLength];

		SpecimenDevice->SetSample(this);

		return SampleData;
	}
}
