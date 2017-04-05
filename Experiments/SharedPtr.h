#pragma once
#include <iostream>
#include "ChunkMemoryAllocator.h"

template <typename T>
class TSharedPtr
{
  T* Obj;
  class CReferenceCounter
  {
    int Cnt = 1;
  public:
    void operator--() { Cnt--; }
    void operator++() { Cnt++; }
    operator bool() const { return Cnt > 0; }
  } *Ref;
  void Destroy() { Free(Ref); Free(Obj); }
public:
  template<typename ... TArgs>
  TSharedPtr(TArgs ... args) : Obj(New<T>(args ...)), Ref(New<CReferenceCounter>()) { }
  TSharedPtr(const TSharedPtr<T> &Other) : Obj(Other.Obj), Ref(Other.Ref) { ++(*Ref); }
  ~TSharedPtr()
  {
    --(*Ref);
    if (!(*Ref))
      Destroy();
  }
  T* operator->() { return Obj; }
};

struct TSharedPtr_test
{
  class Some
  {
    int Number;
  public:
    Some(int Number) : Number(Number) { std::cout << "\nCreated"; }
    ~Some() { std::cout << "\nDestroyed"; }
    void Print() { std::cout << "\nPrint " << Number; }
  };

  TSharedPtr_test()
  {
    TSharedPtr<Some> Ptr1(1);
    TSharedPtr<Some> Ptr2(Ptr1);
    TSharedPtr<Some> Ptr3(2);
    TSharedPtr<Some> Ptr4(3);
    Ptr1->Print();
    Ptr2->Print();
    Ptr3->Print();
    Ptr4->Print();
  }
} _TSharedPtr_test;
