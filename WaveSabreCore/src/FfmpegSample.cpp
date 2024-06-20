#include <WaveSabreCore/FfmpegSample.h>
#include <WaveSabreCore/Helpers.h>

#include <exception>

#include <Windows.h>
#include <strsafe.h>

namespace WaveSabreCore
{
	FfmpegSample::FfmpegSample(char *data, int compressedSize, int uncompressedSize, SampleFormat *format)
		: UncompressedSample(data, compressedSize, uncompressedSize, (SampleFormatHeader *)format, format->Size)
	{
		try
		{
			// Create temporary files

			char tempPath[MAX_PATH];
			char srcPath[MAX_PATH];
			char dstPath[MAX_PATH];

			if (!GetTempPath2A(MAX_PATH, tempPath))
			{
				throw std::exception("GetTempPath failed");
			}

			if (!GetTempFileNameA(tempPath, "WaveSabreSample", 0, srcPath))
			{
				throw std::exception("GetTempFileName failed");
			}

			if (!GetTempFileNameA(tempPath, "WaveSabreSample", 0, dstPath))
			{
				throw std::exception("GetTempFileName failed");
			}


			// Write to source file

			auto hSrcFile = CreateFileA(
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
				throw std::exception("WriteFile failed");
			}

			if (bytesWritten != CompressedSize)
			{
				throw std::exception("Number of bytes written to file does not match expected size");
			}


			// Find ffmpeg in PATH

			char ffmpegPath[MAX_PATH];
			if (!SearchPathA(nullptr, "ffmpeg", ".exe", MAX_PATH, ffmpegPath, nullptr))
			{
				throw std::exception("SearchPath failed");
			}


			// Format command line

			constexpr auto cmdLineSize = 0x800;
			char *cmdLine = new char[cmdLineSize];
			if (StringCbPrintfA(cmdLine, cmdLineSize, "-i %s -f f32le -acodec pcm_f32le %s", srcPath, dstPath) != S_OK)
			{
				throw std::exception("StringCbPrintf failed");
			}


			// Create process

			SECURITY_ATTRIBUTES processAttributes;
			processAttributes.bInheritHandle = true;
			processAttributes.lpSecurityDescriptor = NULL;
			processAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);

			STARTUPINFOA startupInfo;
			memset(&startupInfo, 0, sizeof(startupInfo));
			startupInfo.cb = sizeof(startupInfo);
			// startupInfo.wShowWindow = SW_HIDE;
			// startupInfo.dwFlags |= STARTF_USESHOWWINDOW;

			PROCESS_INFORMATION processInfo;

			auto processOk = CreateProcessA(
				ffmpegPath,
				cmdLine,
				&processAttributes,
				nullptr,
				true,
				0,
				nullptr,
				nullptr,
				&startupInfo,
				&processInfo
			);
			if (!processOk)
			{
				throw std::exception("CreateProcess failed");
			}


			// Wait for process to exit

			WaitForSingleObject(processInfo.hProcess, INFINITE);


			// Read file written by ffmpeg

			auto hDstFile = CreateFileA(
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
				throw std::exception("CreateFile failed");
			}

			auto fileSize = GetFileSize(hDstFile, nullptr);
			if (fileSize != uncompressedSize)
			{
				throw std::exception("File size does not match expected size");
			}

			unsigned long bytesRead;
			if (!ReadFile(hDstFile, SampleData, uncompressedSize, &bytesRead, nullptr))
			{
				throw std::exception("ReadFile failed");
			}

			if (bytesRead != uncompressedSize)
			{
				throw std::exception("Number of bytes read from file does not match expected size");
			}


			// Cleanup

			CloseHandle(hSrcFile);
			CloseHandle(hDstFile);
			DeleteFileA(srcPath);
			DeleteFileA(dstPath);
		}
		catch (const std::exception& e)
		{
			MessageBoxA(0, e.what(), "Failed to load sample with ffmpeg", MB_OK | MB_ICONEXCLAMATION);
		}
	}
}
