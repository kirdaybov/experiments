#pragma once

#include <thread>
#include <mutex>

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

void th_read(Data* data)
{
  for (int i = 0; i < 1000; i++)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    std::lock_guard<std::mutex> guard(g_mutex);
  }
}

struct Threads_test
{
  Threads_test()
  {
    Data data;         
    std::thread th1(th_write, &data, 0);
    std::thread th2(th_read, &data);
    th1.join();
    th2.join();    
  }
} _Threads_test;