#pragma once

#include "MemoryAllocator.h"
#include <cassert>

template<class T, class TAlloc = CAllocator>
class TList
{
  struct TNode
  {
    TNode* _next;
    T _data;
  };
  TNode* _first = nullptr;
  TNode* _last = nullptr;

public:
  ~TList()
  {
    while (_first)
    {
      auto _ = _first->_next;
      delete _first;
      _first = _;
    }
  }
  void add(const T& data)
  {
    TNode* next = new TNode();
    next->_data = data;

    if (!_first)
      _first = next;

    if (_last)
      _last->_next = next;

    _last = next;
  }

  class TIter
  {
    TNode* _current;
  public:
    TIter(TNode* current) : _current(current) {}

    TIter& operator++() { _current = _current->_next; return *this; }
    bool operator==(const TIter& Iter) { return Iter._current == _current; }
    bool operator!=(const TIter& Iter) { return Iter._current != _current; }
    T& operator*() { return _current->_data; }
  };
  TIter begin() { return TIter(_first); }
  TIter end() { return TIter(nullptr); }
};

struct TListTest
{
  TListTest()
  {
    TList<int> List;
    const int N = 10;
    for (int i = 0; i < N; i++)
      List.add(i);

    int i = 0;
    for (int integer : List)
    {
      assert(integer == i);
      i++;
    }
    assert(i == 10);
  }
} _TList;