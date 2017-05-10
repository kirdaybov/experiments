#pragma once
#include "TList.h"

template<class TKey, class TValue>
class TMap
{
  const size_t _table_size = 1024;

  struct TDataUnit
  {
    TKey _key;
    TValue _value;
  };
  TList<TDataUnit> _data[1024];

  size_t get_hash(const TKey &key)
  {
    return (size_t)key;
  }

  TValue& at(const TKey &key)
  {
    size_t idx = get_hash(key) % _table_size;
    for (auto &item : _data[idx])
      if (item._key == key)
        return item._value;

    throw;
  }
public:
  void add(const TKey &key, const TValue &value)
  {
    size_t idx = get_hash(key) % _table_size;
    _data[idx].add({ key, value });
  }

  TValue& operator[](const TKey &key) { return at(key); }
};

struct MapTest
{
  MapTest()
  {
    TMap<int, char> map;
    TList<char> list;
    for (int i = 0; i < 10; i++)
    {
      char c = rand() % 255;
      map.add(i, c);
      list.add(c);
    }

    auto Iter = list.begin();
    for (int i = 0; i < 10; i++)
    {
      assert(map[i] == *Iter);
      ++Iter;
    }
  }
} _TMapTest;