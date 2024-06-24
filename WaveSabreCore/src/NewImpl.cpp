#include <new>
#include <emscripten.h>

// Simple bump alloc. which leaks memory
static char buffer[20 * 1024*1024]; // heap memory
static int offset;

extern "C"
{
	extern void* WaveSabre_Alloc(std::size_t size);
}

void* operator new(std::size_t size)
{
	return WaveSabre_Alloc(size);
}

void* operator new[](std::size_t size)
{
	return WaveSabre_Alloc(size);
}

void operator delete(void *p) noexcept
{
	//...
}

void operator delete[](void *p) noexcept
{
	// TODO: implement
}

#include "cxxabi.h"
extern "C"
{
	_LIBCXXABI_FUNC_VIS _LIBCXXABI_NORETURN
	void __cxa_pure_virtual(void)
	{
		// abort_message("Pure virtual function called!");
	}

	_LIBCXXABI_FUNC_VIS _LIBCXXABI_NORETURN
	void __cxa_deleted_virtual(void)
	{
		// abort_message("Deleted virtual function called!");
	}
}