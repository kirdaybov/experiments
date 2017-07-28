#pragma once

#define MAX_TEST_NUM 1024

struct Tester
{
  typedef void (*TestFunc)();

  void run() {
    for (int i = 0; i < size; i++)
      (*tests[i])();
  }

  void add_test(TestFunc func)
  {
    if (size >= MAX_TEST_NUM - 1) return; // TODO: error
    tests[size] = func;
    size++;
  }

  static Tester* get() { if (!tester) tester = new Tester(); return tester; }
private:
  static Tester* tester;
  int size = 0;
  TestFunc tests[MAX_TEST_NUM];
};

#define RUN_TESTS Tester::get()->run();
#define ADD_TEST(Name, Body)                                              \
void Name##_test_impl()                                                   \
{                                                                         \
  Body                                                                    \
}                                                                         \
struct Name##_test_adder                                                  \
{                                                                         \
  Name##_test_adder() { Tester::get()->add_test(&Name##_test_impl); }     \
} Name##_test_adder_var;
