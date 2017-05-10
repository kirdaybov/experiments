#pragma once
#include <cassert>
#include <iostream>

#include "MemoryAllocator.h"

template<class T, class TAlloc = CAllocator>
class TArray
{
  size_t _capacity = 10;
  size_t _grow_size = 10;
  size_t _size = 0;
  T* _data = nullptr;

  void grow() {
    T* new_data = static_cast<T*>(TAlloc::alloc((_capacity + _grow_size)*sizeof(T)));
    memcpy(new_data, _data, _capacity*sizeof(T));
    TAlloc::free(_data);
    _data = new_data;
    _capacity += _grow_size;
  }
  inline void check_grow() { if (_size == _capacity) grow(); }
public:
  TArray() : _data(static_cast<T*>(TAlloc::alloc(_capacity*sizeof(T)))) {}
  TArray(size_t capacity) : _capacity(capacity), _data(static_cast<T*>(TAlloc::alloc(capacity * sizeof(T)))) {}
  TArray(size_t capacity, const T& value) : _capacity(capacity), _data(static_cast<T*>(TAlloc::alloc(capacity * sizeof(T)))) { for (int i = 0; i < _capacity; i++) add(value); }
  ~TArray() { TAlloc::free(_data); }

  inline size_t size() { return _size; }

  void add(const T& El) {
    check_grow();
    memcpy(_data + _size, &El, sizeof(T));
    _size++;
  }

  void remove_last() {
    _size--;
  }

  void remove_at(size_t i) {
    if (i < _size - 1)
    {
      memmove(_data + i, _data + i + 1, sizeof(T)*(_size-i-1));
      _size--;
    }      
    else
      remove_last(); 
  }

  inline T& at(size_t i) { return *(_data + i); }
  T& operator [] (size_t i) { return at(i); }
  int find(const T& el) 
  { 
    for (int i = 0; i < _size; i++)
      if (at(i) == el)
        return i;
    return -1;
  }

  class TIter
  {
    TArray<T>* _arr;
    size_t _current = 0;
  public:
    TIter(TArray<T>* arr) : _arr(arr) {}
    TIter(TArray<T>* arr, size_t current) : _arr(arr), _current(current) {}
    //operator bool() { return _arr->size > 0 && _current < _arr->size; }
    TIter& operator++() { _current++; return *this; }
    bool operator==(const TIter& Iter) { return Iter._current == _current; }
    bool operator!=(const TIter& Iter) { return Iter._current != _current; }
    T& operator*() { return (*_arr)[_current]; }
  };
  TIter begin() { return TIter(this); }
  TIter end() { return TIter(this, _size); }
};

template<class T>
class TDArray : private TArray<T>
{
  using TArray<T>::add;
public:
  void d_add(const T& El) { TArray<T>::add(El); }
  using TArray<T>::size;
  using TArray<T>::operator[];
};

struct TArrayTest
{
  TArrayTest()
  {
    TArray<int> integers;

    for (auto integer : integers)
      assert(false);
    const int N = 10;
    for (int i = 0; i < N; i++)
      integers.add(i);
    assert(integers.size() == N);
    for (int i = 0; i < N; i++)
    {
      assert(integers[i] == i);
    }

    int i = 0;
    for (auto integer : integers)
    {
      assert(integer == i);
      i++;
    }
    assert(i == N);

    for (int i = 0; i < N / 2; i++)
      integers.remove_last();
    integers.remove_at(0);
    integers.remove_at(1);
    assert(integers.size() == 3);

    assert(integers[0] == 1);
    assert(integers[1] == 3);
    assert(integers[2] == 4);
  }
} _TArrayTest;

struct TDArrayTest
{
  TDArrayTest()
  {
    TDArray<int> integers;

    const int N = 10;
    for (int i = 0; i < N; i++)
      integers.d_add(i);
    assert(integers.size() == N);    
  }
} _TDArrayTest;