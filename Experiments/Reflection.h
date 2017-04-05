#pragma once
#include <cassert>

struct Type;

bool operator ==(const Type& a, const Type& b)
{
  if (&a == &b) return true; // baaad  
}

struct Type
{
  int Id = 0;
  Type* Parent = nullptr;
  bool IsChildOf(const Type& b) const
  {
    if (operator ==(b, *this))
      return true;

    if (Parent)
      return IsChildOf(*Parent);
    return false;
  }
};

struct Types
{

};

#define GENERATE_REFLECTION(ClassName) static const Type& Class() { static Type Class##ClassName; return Class##ClassName; }
#define GENERATE_REFLECTION_INHERITED(ClassName, ParentClassName) static const Type& Class() { static Type Class##ClassName; static Type Class##ParentName; Class##ClassName.Parent = &Class##ParentName; return Class##ClassName; }

//template <class T>
//struct TType : public Type
//{
//  static Type* Class()
//  {
//    static TType<T> ThisClass;
//    return &ThisClass##T;
//  }
//};

struct AClass
{
  GENERATE_REFLECTION(AClass)
};

struct BClass
{
  GENERATE_REFLECTION(BClass)
};

struct BInheritedClass : public BClass
{
  GENERATE_REFLECTION_INHERITED(BInheritedClass, BClass)
};


struct Reflection_test
{
  Reflection_test()
  {
    AClass a1 = AClass();
    AClass a2 = AClass();
    BClass b1 = BClass();
    BInheritedClass b2 = BInheritedClass();

    assert(a1.Class().IsChildOf(a2.Class()));
    assert(b2.Class().IsChildOf(b1.Class()));
    assert(!b2.Class().IsChildOf(a1.Class()));
  }
} _Reflection_test;