#ifndef __WAVESABRECORE_UNCOMPRESSEDSAMPLE_H__
#define __WAVESABRECORE_UNCOMPRESSEDSAMPLE_H__

namespace WaveSabreCore
{
	union SampleFormatHeader;

	class UncompressedSample
	{
	public:
		UncompressedSample(
			char *compressedData,
			int compressedSize,
			SampleFormatHeader *formatHeader,
			int formatSize
		);

		~UncompressedSample();

		int FormatHeaderSize;
		int CompressedSize;
		int UncompressedSize;
		int SampleLength;

		SampleFormatHeader *FormatHeader;
		char *CompressedData;
		float *SampleData;
	};
}

#endif
