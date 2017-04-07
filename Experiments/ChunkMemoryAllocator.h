#pragma once
#include <stack>

class CMemoryAllocator
{
  static CMemoryAllocator* Instance;
  static const size_t ChunkSize;
  static const size_t ChunkNumber;
  int* Storage = nullptr;

  struct ChunkInfo
  {
    void* Ptr;
  };

  std::stack<ChunkInfo> Chunks;

public:
  CMemoryAllocator()
  {
    Storage = (int*)malloc(ChunkNumber * ChunkSize);
    for (size_t i = 0; i < ChunkNumber; i++)
      Chunks.push({ Storage + i });
  }
  ~CMemoryAllocator()
  {
    free(Storage);
  }

  __forceinline static CMemoryAllocator* Get() { return Instance; }
  template<typename T, typename ... Args>
  __forceinline T* Alloc(Args ... args) { return new (Alloc(sizeof(T))) T(args ...); }
  __forceinline void* Alloc(size_t Size) { auto Ptr = Chunks.top().Ptr; Chunks.pop(); return Ptr; }
  __forceinline void Free(void *Ptr) { Chunks.push({ Ptr }); }
};

template<typename T, typename ... TArgs>
T* New(TArgs ... args) { return CMemoryAllocator::Get()->Alloc<T>(args ...); }
template<typename T>
void Free(T* Ptr) { Ptr->~T(); CMemoryAllocator::Get()->Free(Ptr); }