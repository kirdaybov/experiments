#include <iostream>
#include <stack>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <fstream>
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
  void Print() { std::cout << std::endl << "Value: " << Value; }
private:
  T Value;
};

template<typename T>
class Delegate
{
  void BindStatic(T Func) { cb = Callback<T>(Func); IsBound = true; }
  template<typename O> void BindMember(O* o, T Func) { cb = Callback<T>(o, Func); IsBound = true; }
private:
  bool IsBound = false;
  Callback<T> cb;
};

#define DELEGATE(Name) typedef Delegate<void(*)()> Name;
#define DELEGATE_One(Name, TParam1) typedef Delegate<void(*)(TParam1)> Name;
#define DELEGATE_Ret_One(Name, TParam1) typedef Delegate<Ret(*)(TParam1)> Name;

template <typename T> Container<T> MakeContainer(const T& t) { return Container<T>(t); }
template <typename T> Callback<T> MakeCallback(T t) { return Callback<T>(t); }
template <typename O, typename T> Callback<T> MakeMemberCallback(O* o, T t) { return Callback<T>(o, t); }

template<typename T> void Print(const T& t) { std::cout << t; }

void Hello()
{
  std::cout << "Hello";
}

int Square(int x) { return x*x; }

class Member
{
public:
  int Something = 3;

  void Print()
  {
    std::cout << std::endl << "Value: " << Member::Something << std::endl << "Address: " << &Member::Something << std::endl;
  }
};

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
  //std::map<
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

template <typename T>
class CArray
{
public:
  void Add(const T &El) {};
  T& operator [] (int Index) {}

  T* begin() { return &operator[0]; }
  const T* begin() const { return &operator[0]; }
  T* end() { return &operator[0]; }
  const T* end() const { return &operator[0]; }
};

class CMemoryAllocator
{
  static CMemoryAllocator* Instance;
  static const size_t ChunkSize;
  static const uint16_t ChunkNumber;
  int* Storage = nullptr;

  struct ChunkInfo
  {
    void* Ptr;
  };

  std::stack<ChunkInfo> Chunks;

public:
  CMemoryAllocator()
  {
    Storage = (int*)malloc(ChunkNumber * ChunkSize);
    for (int i = 0; i < ChunkNumber; i++)
      Chunks.push({ Storage + i });
  }
  ~CMemoryAllocator()
  {
    free(Storage);
  }

  __forceinline static CMemoryAllocator* Get() { return Instance; }
  template<typename T, typename ... Args>
  __forceinline T* Alloc(Args ... args) { return new (Alloc(sizeof(T))) T(args ...); }
  __forceinline void* Alloc(size_t Size) { auto Ptr = Chunks.top().Ptr; Chunks.pop(); return Ptr; }
  __forceinline void Free(void *Ptr) { Chunks.push({ Ptr }); }
};

template<typename T, typename ... TArgs>
T* New(TArgs ... args) { return CMemoryAllocator::Get()->Alloc<T>(args ...); }
template<typename T>
void Free(T* Ptr) { Ptr->~T(); CMemoryAllocator::Get()->Free(Ptr); }


CMemoryAllocator* CMemoryAllocator::Instance = new CMemoryAllocator();
const size_t CMemoryAllocator::ChunkSize = 64;
const uint16_t CMemoryAllocator::ChunkNumber = 200;

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

class Some
{
  int Number;
public:
  Some(int Number) : Number(Number) { std::cout << "\nCreated"; }
  ~Some() { std::cout << "\nDestroyed"; }
  void Print() { std::cout << "\nPrint " << Number; }
};

struct Data
{
  int x = 0;
  int y = 0;
};

std::mutex g_mutex;

void th_write(Data* data, int num)
{
  for (int i = 0; i < 1000; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    num = rand() % 10;
    std::lock_guard<std::mutex> guard(g_mutex);
    data->x = num;
    data->y = num;
  }
}

void th_read(Data* data, std::ofstream* ofs)
{
  for (int i = 0; i < 1000; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    std::lock_guard<std::mutex> guard(g_mutex);
    *ofs << "\n" << data->x;
    *ofs << " " << data->y;
  }
}

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
void MoreHello(int i)
{
  std::cout << "\nHello, " << i;
}

template <typename TFunc, typename ... TArgs>
void Func(TFunc Func, TArgs ... Args)
{
  (*Func)(Args...);
}

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

//struct s1 { s1() { std::cout << "\ns1"; } };
//struct s2 : s1 { s2() { std::cout << "\ns2"; } };
//struct s3 : s2 { s3() { std::cout << "\ns3"; } };
//struct D : s1, s2, private s3 { D() { std::cout << "\nD"; } };

//template <class TNode>
//class CGraph
//{
//  class CEdge
//  {
//    TNode* _start;
//    TNode* _end;
//  };
//  std::vector<TNode> _nodes;
//  std::vector<CEdge> _nodes;
//};

class CAllocator
{
public:
  static void* alloc(size_t size) { return malloc(size); }
  static void free(void* ptr) { ::free(ptr); }
};

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

template<class T>
class TGraph
{
  class TVertice
  {
    T data;
  };
  class TEdge
  {
    TVertice* _start;
    TVertice* _end;
  };

  TArray<TVertice> _vertices;
  TArray<TEdge> _edges;
public:
};

struct GraphTest
{
  GraphTest()
  {
    TGraph<int> graph;
  }
} _TGraph;

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
    std::cout << std::endl;
    for (int i = 0; i < 10; i++)
    {
      char c = rand() % 255;
      map.add(i, c);
      std::cout << c;
    }

    std::cout << std::endl;
    for (int i = 0; i < 10; i++)
    {
      std::cout << map[i];
    }
  }
} _TMapTest;

int main()
{
  std::cout << std::endl << "Hi!";
  system("PAUSE");
  return 0;

  //D* d = new D();
  ////s1* ps1 = d;
  ////s2* ps2 = d;
  ////s3* ps3 = d;
  //std::cout << "\n" << sizeof(s1);
  //std::cout << "\n" << sizeof(s2);
  //std::cout << "\n" << sizeof(s3);
  //std::cout << "\n" << sizeof(D);
  //system("PAUSE");
  //return 0;

  for (auto i : Iterator(0, 0, 10))
    std::cout << i;

  Func(Hello);
  Func(MoreHello, 4);
  system("PAUSE");
  return 0;

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
  {
    Data data;
    //for (int i = 0; i < 100; i++)
    {
      std::ofstream ofs("C:\\_logs\\res.txt", std::ofstream::out);
      std::thread th1(th_write, &data, 0);
      std::thread th2(th_read, &data, &ofs);
      th1.join();
      th2.join();
    }
  }
  system("PAUSE");
  return 0;

  auto Call = MakeCallback(Hello);
  auto CallSquare = MakeCallback(Square);
  Call();
  std::cout << CallSquare(3);

  auto Con = MakeContainer(3);

  auto MCall = MakeMemberCallback(&Con, &Container<int>::Print);
  MCall();

  AClass a1 = AClass();
  AClass a2 = AClass();
  BClass b1 = BClass();
  BInheritedClass b2 = BInheritedClass();

  if (!a1.Class().IsChildOf(a2.Class())) std::cout << "Fail 1!";
  if (!b2.Class().IsChildOf(b1.Class())) std::cout << "Fail 2!";
  if (b2.Class().IsChildOf(a1.Class())) std::cout << "Fail 3!";

  system("PAUSE");
  return 0;
}