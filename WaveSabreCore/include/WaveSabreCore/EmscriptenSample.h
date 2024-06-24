#pragma once

#include "SampleFormat.h"
#include "UncompressedSample.h"

#include <emscripten.h>

namespace WaveSabreCore
{
	class Specimen;

	class EmscriptenSample : public UncompressedSample
	{
	public:
		EmscriptenSample(char *compressedData, int compressedSize, SampleFormat *format, Specimen *specimen);
		void BeginUncompress();
		float* FinishUncompress(int sampleLength);

		Specimen *SpecimenDevice;
	};
}

extern "C"
{
	extern void WaveSabre_BeginUncompressSample(
		WaveSabreCore::EmscriptenSample *sample,
		char *compressedData,
		int compressedSize
	);

	EMSCRIPTEN_KEEPALIVE
	float* WaveSabre_FinishUncompressSample(
		WaveSabreCore::EmscriptenSample *sample,
		int sampleLength
	);
}
