#pragma once
#include <cassert>

int func(int N)
{
  const int num_bits = sizeof(int) * 8;
  size_t longest = 0;
  size_t current = 0;
  bool bStarted = false;
  for (int i = 0; i < num_bits; i++)
  {
    int bit = (N << (num_bits - 1 - i)) >> (num_bits - 1);
    if (bit == 0)
    {
      if (bStarted)
        current++;
    }
    else
    {
      if (current > longest)
        longest = current;
      current = 0;
      bStarted = true;
    }
  }
  return longest;
}

struct t_func
{
  t_func()
  {
    assert(func(129) == 6);
    assert(func(128) == 0);
    assert(func(137) == 3);
    assert(func(145) == 3);
  }
} _;