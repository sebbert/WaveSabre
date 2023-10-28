#include <new>

// Simple bump alloc. which leaks memory
char buffer[1024*1024]; // 20 mb buffer for heap
int off;

void * operator new(std::size_t n)
{
  auto startOfBlock = buffer + off;
  off += n;
  return (void*)startOfBlock;
}
void *operator new[](std::size_t s)
{
  auto startOfBlock = buffer + off;
  off += s;
  return (void*)startOfBlock;
}

void operator delete(void * p) noexcept
{
  //...
}
void operator delete[](void *p) noexcept
{
  // TODO: implement
}