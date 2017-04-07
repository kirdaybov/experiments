#pragma once
#include <cassert>

//class MyClass
//{
//public:
//  void MyFunc(int i) {}
//};
//
//template<class T, class ... TArgs> 
//class TFunctor
//{
//  typedef void (T::*TFunc)(TArgs ...);
//};
//
//template<class T, class ... TArgs>
//class MyFunctor<T, void (T::*Func)(TArgs...), TArgs>
//{
//  void operator () (T* Obj, )
//};
//
//template<class T, TFunc Func>
//void CallTFuncOnT(T* Obj)
//{
//  Obj->(*Func)();
//}
//
//
//template <void (*TFunc)()>
//void Func()
//{
//
//}
//
//Func<MyClass, &MyClass::Func>(4);
bool Hello() { return true; }
int MoreHello(int i)
{
  return i;
}

template <typename TFunc, typename ... TArgs>
void Func(TFunc Func, TArgs ... Args)
{
  (*Func)(Args...);
}

struct IndirectFunctionCall_test
{
  IndirectFunctionCall_test()
  {
    Func(Hello);
    Func(MoreHello, 4);
  }  
} _IndirectFunctionCall_test;