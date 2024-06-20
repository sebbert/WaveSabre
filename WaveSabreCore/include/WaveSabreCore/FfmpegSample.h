#pragma once

#include "UncompressedSample.h"
#include "SampleFormat.h"

namespace WaveSabreCore
{
	class FfmpegSample : public UncompressedSample
	{
	public:
		FfmpegSample(char *data, int compressedSize, int uncompressedSize, SampleFormat *format);
	};
}