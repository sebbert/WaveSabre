#ifndef __WAVESABRECORE_WAVEFORMAT_H__
#define __WAVESABRECORE_WAVEFORMAT_H__

#ifdef _WIN32
	#include <Windows.h>

	#ifndef UNICODE
		#define _UNICODE
	#endif

	#include <mmreg.h>
#endif

namespace WaveSabreCore
{
	enum class SampleFormatType : unsigned int
	{
		Blob = 0,
	};

	struct SampleFormat
	{
		// Version 0:
		unsigned short Tag; // Must be 0
		unsigned short Version; // Version number of this SampleFormat structure
		int Size; // sizeof(SampleFormat) when the chunk was saved
		SampleFormatType Type;

		static constexpr unsigned short kTag = 0;
		static constexpr unsigned short kVersion = 0;
	};

	// Following ChunkHeader is either a WAVEFORMATEX or SampleFormat.
	//  We use the 16 bit format tag at the start of WAVEFORMATEX as a discriminator, which we also include at the start of SampleFormat.
	//  If the format tag is 0 (WAVE_FORMAT_UNKNOWN) it is an SampleFormat, otherwise it is a WAVEFORMATEX.
	union SampleFormatHeader
	{
		unsigned short Tag;
		SampleFormat SampleFormat;

#ifdef _WIN32
		WAVEFORMATEX WaveFormat;
#endif
	};
}

#endif
