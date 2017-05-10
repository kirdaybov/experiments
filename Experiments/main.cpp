//#include "ChunkMemoryAllocator.h"
//#include "Delegates.h"
//#include "IndirectFunctionCall.h"
//#include "Reflection.h"
//#include "SharedPtr.h"
//#include "Threads.h"

//#include "MemoryAllocator.h"
//#include "ShortTasks.h"
#include "TArray.h"
#include "TGraph.h"
//#include "TList.h"
//#include "TMap.h"

#include <iostream>
#include <vector>
#include <cassert>

struct A
{
  int i = 0;
};

void* operator new(unsigned int, A &a)
{
  A* pa = new A();
  pa->i = a.i;
  return pa;
}

template<typename T> void Print(const T& t) { std::cout << t; }

class Member
{
public:
  int Something = 3;

  void Print()
  {
    std::cout << std::endl << "Value: " << Member::Something << std::endl << "Address: " << &Member::Something << std::endl;
  }
};

template<int I>
struct Factorial
{
  enum { value = I*Factorial<I - 1>::value };
};

template<>
struct Factorial<0>
{
  enum { value = 1 };
};

template<int A, int B>
struct Sum
{
  enum { value = A + B };
};

#define STR(x) STR2(x)
#define STR2(x) #x

#define MESSAGE(x) __FILE__ " line " STR(__LINE__) ": " x

struct Iterator
{
  Iterator(int aindex, int astart, int aend) : index(aindex), _start(astart), _end(aend) {}
  int index = 0;

  int _start = 0;
  int _end = 10;

  bool operator != (Iterator &Other) { return Other.index != index; }
  int operator*() { return index; }
  Iterator& operator++() { std::cout << "\n++"; index++; return *this; }
  Iterator begin() { std::cout << "\nbegin"; return Iterator(_start, _start, _end); }
  Iterator end() { std::cout << "\nend"; return Iterator(_end, _start, _end); }
};

int main()
{  
  system("PAUSE");
  return 0;
}