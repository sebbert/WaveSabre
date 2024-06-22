#include <WaveSabreCore/UncompressedSample.h>

#include <string.h>

namespace WaveSabreCore
{
	UncompressedSample::UncompressedSample(
		char *compressedData,
		int compressedSize,
		SampleFormatHeader *formatHeader,
		int formatHeaderSize
	)
		: CompressedSize(compressedSize)
		, FormatHeaderSize(formatHeaderSize)
	{
		CompressedData = new char[compressedSize];
		memcpy(CompressedData, compressedData, compressedSize);

		FormatHeader = (SampleFormatHeader *) new char[formatHeaderSize];
		memcpy(FormatHeader, formatHeader, formatHeaderSize);
	}

	UncompressedSample::~UncompressedSample()
	{
		delete [] (char *)FormatHeader;
		delete [] CompressedData;

		if (SampleData) delete [] SampleData;
	}
}