#pragma once

#include "UncompressedSample.h"
#include "SampleFormat.h"

namespace WaveSabreCore
{
	// An audio sample which is uncompressed by invoking ffmpeg on the command line.
	//  This should only be used for VSTs, not in export.
	class FfmpegSample : public UncompressedSample
	{
	public:
		FfmpegSample(char *data, int compressedSize, SampleFormat *format);
	};
}