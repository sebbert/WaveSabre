#include <WaveSabreCore/UncompressedSample.h>

#include <string.h>

namespace WaveSabreCore
{
	UncompressedSample::UncompressedSample(
		char *compressedData,
		int compressedSize,
		int uncompressedSize,
		SampleFormatHeader *formatHeader,
		int formatHeaderSize
	)
		: CompressedSize(compressedSize)
		, UncompressedSize(uncompressedSize)
		, FormatHeaderSize(formatHeaderSize)
	{
		CompressedData = new char[compressedSize];
		memcpy(CompressedData, compressedData, compressedSize);

		FormatHeader = (SampleFormatHeader *) new char[formatHeaderSize];
		memcpy(FormatHeader, formatHeader, formatHeaderSize);

		SampleLength = uncompressedSize / (int)sizeof(float);
		SampleData = new float[SampleLength];
	}

	UncompressedSample::~UncompressedSample()
	{
		delete [] (char *)FormatHeader;
		delete [] CompressedData;
		delete [] SampleData;
	}
}