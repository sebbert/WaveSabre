#include <new>

// Simple bump alloc. which leaks memory
static char buffer[1024*1024]; // 20 mb buffer for heap
static int offset;

void * bumpAlloc(std::size_t size)
{	
	auto startOfBlock = buffer + offset;
	offset += size;
	return (void*)startOfBlock;
}

void * operator new(std::size_t size)
{
	return bumpAlloc(size);
}

void * operator new[](std::size_t size)
{
	return bumpAlloc(size);
}

void operator delete(void * p) noexcept
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