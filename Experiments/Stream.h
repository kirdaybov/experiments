#pragma once
#include <cassert>
#include <iostream>

#include "MemoryAllocator.h"

// byte stream

template<class TAlloc = CAllocator>
class Stream
{
  static constexpr size_t capacity_default = 10;

  char* data_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
  char* current_ = nullptr;

public:
  Stream() : data_(TAlloc::alloc(capacity_default)), size(0), capacity(capacity_default), current_(data_) {}

  template<typename T> void serialize(T& value)
  {
    size_t piece_size = sizeof(T);
    *(T*)current_ = value;
    current_ += piece_size;
    size_ += piece_size;
  }

  template<typename T> void deserialize(T& value)
  {
    size_t piece_size = sizeof(T);
    assert(size >= piece_size);
    value = *(T*)current;
    current += piece_size;
    size -= piece_size;
  }
};