#pragma once
#include <cassert>

template<typename TFunc>
class Callback
{};

template<typename Ret, typename... Args>
class Callback<Ret(*)(Args...)>
{
public:
  typedef Ret(*TFunc)(Args...);
  Callback(TFunc Func) : Func(Func) {}

  Ret operator ()(Args... args)
  {
    return Func(args...);
  }
private:
  TFunc Func;
};

template<typename T, typename Ret, typename... Args>
class Callback<Ret(T::*)(Args...)>
{
public:
  typedef Ret(T::*TFunc)(Args...);
  Callback(T* Object, TFunc Func) : Object(Object), Func(Func) {}

  Ret operator ()(Args... args)
  {
    return (Object->*Func)(args...);
  }
private:
  T* Object;
  TFunc Func;
};

template<typename T>
class Container
{
public:
  Container(const T& t) : Value(t) {}
  T Get() { return Value; }
private:
  T Value;
};

template<typename TFunc>
class Delegate {};

template<typename Ret, typename... Args>
class Delegate<Ret (Args...)>
{
  typedef 
public:
  void BindStatic(T Func) { cb = Callback<T>(Func); IsBound = true; }
  template<typename O> void BindMember(O* o, T Func) { cb = Callback<T>(o, Func); IsBound = true; }
  Ret Execute(Args... args) { return cb(args...); }
private:
  bool IsBound = false;
  Callback<T> cb;
};

#define DELEGATE(Name) typedef Delegate<void(*)()> Name;
#define DELEGATE_One(Name, TParam1) typedef Delegate<void(*)(TParam1)> Name;
#define DELEGATE_Ret_One(Name, Ret, TParam1) typedef Delegate<Ret(*)(TParam1)> Name;

template <typename T> Container<T> MakeContainer(const T& t) { return Container<T>(t); }
template <typename T> Callback<T> MakeCallback(T t) { return Callback<T>(t); }
template <typename O, typename T> Callback<T> MakeMemberCallback(O* o, T t) { return Callback<T>(o, t); }

struct Delegate_test
{
  static bool Hello() { return true; }
  static int Square(int x) { return x*x; }

  DELEGATE_Ret_One(DHandleEvent, int, int)
  DHandleEvent event_handler;

  Delegate_test()
  {
    event_handler.BindStatic(Square);
    assert(event_handler.Execute(3) == 9);
    auto Call = MakeCallback(Hello);
    auto CallSquare = MakeCallback(Square);
    assert(Call());
    assert(CallSquare(3) == 9);
    
    auto Con = MakeContainer(3);
    auto MCall = MakeMemberCallback(&Con, &Container<int>::Get);
    assert(MCall() == 3);
  }
} _Delegate_test;