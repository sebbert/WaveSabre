#pragma once

#include "SampleFormat.h"
#include "UncompressedSample.h"

namespace WaveSabreCore
{
	class ExternalSample : public UncompressedSample
	{
	public:
		ExternalSample(char *CompressedData, int compressedSize, SampleFormat *format);
	};
}