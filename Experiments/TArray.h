#pragma once
#include <cassert>

#include "MemoryAllocator.h"

template<class T, class TAlloc = CAllocator>
class TArray
{
  size_t capacity = 10;
  size_t grow_size = 10;
  size_t _size = 0;
  T* _data = nullptr;

  void grow() {
    T* new_data = static_cast<T*>(TAlloc::alloc((capacity + grow_size)*sizeof(T)));
    memcpy(new_data, _data, capacity*sizeof(T));
    TAlloc::free(_data);
    _data = new_data;
    capacity += grow_size;
  }
  inline void check_grow() { if (_size == capacity) grow(); }
public:
  TArray() : _data(static_cast<T*>(TAlloc::alloc(capacity*sizeof(T)))) {}
  ~TArray() { TAlloc::free(_data); }

  inline size_t size() { return _size; }

  void add(const T& El) {
    check_grow();
    memcpy(_data + _size, &El, sizeof(T));
    _size++;
  }
  void remove_at(int i) {
    //*(_data + size*sizeof(T)) = El; 
    //size++; 
  }
  inline T& at(size_t i) { return *(_data + i); }
  T& operator [] (size_t i) { return at(i); }

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
  TIter end() { return TIter(this, _size > 0 ? _size - 1 : 0); }
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
    std::cout << std::endl << "TArrayTest Start";
    TArray<int> integers;

    const int N = 10;
    for (int i = 0; i < N; i++)
      integers.add(i);
    assert(integers.size() == N);
    for (int i = 0; i < N; i++)
      assert(integers[i] == i);

    int i = 0;
    for (auto integer : integers)
    {
      assert(integer == i);
      i++;
    }
    std::cout << std::endl << "TArrayTest End";
  }
} _TArrayTest;

struct TDArrayTest
{
  TDArrayTest()
  {
    std::cout << std::endl << "TDArrayTest Start";
    TDArray<int> integers;

    const int N = 10;
    for (int i = 0; i < N; i++)
      integers.d_add(i);
    assert(integers.size() == N);
    for (int i = 0; i < N; i++)
      std::cout << std::endl << integers[i];

    //for (auto integer : integers)
    //  std::cout << std::endl << integer;
    std::cout << std::endl << "TDArrayTest End";
  }
} _TDArrayTest;