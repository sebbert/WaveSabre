#include <WaveSabreCore/MxcsrFlagGuard.h>

#if defined(_M_IX86) || defined(_M_X64)
#	define IS_X86_OR_X64
#endif

#ifdef IS_X86_OR_X64
#include <xmmintrin.h>
#include <pmmintrin.h>
#endif

namespace WaveSabreCore
{
	MxcsrFlagGuard::MxcsrFlagGuard()
	{
#ifdef IS_X86_OR_X64
		mxcsrRestore = _mm_getcsr();

		// Set SSE FTZ and DAZ flags
		_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
		_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif
	}

	MxcsrFlagGuard::~MxcsrFlagGuard()
	{
#ifdef IS_X86_OR_X64
		_mm_setcsr(mxcsrRestore);
#endif
	}
}
