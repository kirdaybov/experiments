#pragma once

class CAllocator
{
public:
  static void* alloc(size_t size) { return malloc(size); }
  static void free(void* ptr) { ::free(ptr); }
};