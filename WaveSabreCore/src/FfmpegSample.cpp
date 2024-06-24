#include <WaveSabreCore/FfmpegSample.h>
#include <WaveSabreCore/Helpers.h>

#include <Windows.h>
#include <strsafe.h>

namespace WaveSabreCore
{
	static void ReportError(TCHAR *msg)
	{
		MessageBox(0, msg, TEXT("Failed to load sample with ffmpeg"), MB_OK | MB_ICONEXCLAMATION);
	}

	FfmpegSample::FfmpegSample(char *data, int compressedSize, SampleFormat *format)
		: UncompressedSample(data, compressedSize, (SampleFormatHeader *)format, format->Size)
	{
		// Create temporary files

		TCHAR tempPath[MAX_PATH];
		TCHAR srcPath[MAX_PATH];
		TCHAR dstPath[MAX_PATH];

		if (!GetTempPath(MAX_PATH, tempPath))
		{
			ReportError(TEXT("GetTempPath failed"));
			return;
		}

		if (!GetTempFileName(tempPath, TEXT("WVS"), 0, srcPath))
		{
			ReportError(TEXT("GetTempFileName failed"));
			return;
		}

		if (!GetTempFileName(tempPath, TEXT("WVS"), 0, dstPath))
		{
			ReportError(TEXT("GetTempFileName failed"));
			return;
		}


		// Write to source file

		auto hSrcFile = CreateFile(
			srcPath,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			0
		);

		unsigned long bytesWritten;
		if (!WriteFile(hSrcFile, CompressedData, CompressedSize, &bytesWritten, nullptr))
		{
			ReportError(TEXT("WriteFile failed"));
			return;
		}

		if (bytesWritten != CompressedSize)
		{
			ReportError(TEXT("Number of bytes written to file does not match expected size"));
			return;
		}


		// Find ffmpeg in PATH

		TCHAR ffmpegPath[MAX_PATH];
		if (!SearchPath(nullptr, TEXT("ffmpeg"), TEXT(".exe"), MAX_PATH, ffmpegPath, nullptr))
		{
			ReportError(TEXT("SearchPath failed"));
			return;
		}


		// Format command line

		constexpr auto cmdLineSize = 0x400;
		auto cmdLine = new TCHAR[cmdLineSize];
		if (StringCbPrintf(
			cmdLine,
			cmdLineSize,
			TEXT(
				"ffmpeg.exe"
				
				// Input file path
				" -i \"%s\""

				// Select first audio channel only
				" -af pan=1|c0=c0"

				// Resample to 44.1 KHz
				" -ar 44100"

				// Encode as raw 32 bit float, little endian
				" -f f32le"
				" -acodec pcm_f32le"

				// Overwrite output file
				" -y"

				// Output file path
				" \"%s\""
			),
			srcPath,
			dstPath
		) != S_OK)
		{
			ReportError(TEXT("StringCbPrintf failed"));
			delete[] cmdLine;
			return;
		}


		// Create process

		STARTUPINFO startupInfo;
		memset(&startupInfo, 0, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
		startupInfo.wShowWindow = SW_HIDE;

		PROCESS_INFORMATION processInfo;
		auto processOk = CreateProcess(ffmpegPath, cmdLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo);
		delete[] cmdLine;
		if (!processOk)
		{
			ReportError(TEXT("CreateProcess failed"));
			return;
		}


		// Wait for process to exit

		WaitForSingleObject(processInfo.hProcess, INFINITE);


		// Open file written to by ffmpeg

		auto hDstFile = CreateFile(
			dstPath,
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0
		);
		if (hDstFile == INVALID_HANDLE_VALUE)
		{
			ReportError(TEXT("CreateFile failed"));
			return;
		}


		// Calculate size

		auto fileSize = GetFileSize(hDstFile, nullptr);
		UncompressedSize = fileSize;
		SampleLength = fileSize / (int)sizeof(float);
		SampleData = new float[SampleLength];


		// Read from destination file

		unsigned long bytesRead;
		if (!ReadFile(hDstFile, SampleData, fileSize, &bytesRead, nullptr))
		{
			ReportError(TEXT("ReadFile failed"));
			return;
		}

		if (bytesRead != fileSize)
		{
			ReportError(TEXT("Number of bytes read from file does not match file size"));
			return;
		}


		// Cleanup

		CloseHandle(hSrcFile);
		CloseHandle(hDstFile);
		DeleteFile(srcPath);
		DeleteFile(dstPath);
	}
	
}
