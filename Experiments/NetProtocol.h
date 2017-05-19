#pragma once

#include <cassert>
#include <string.h>

typedef unsigned int uint;

const uint protocol_id = 0x1;

enum class net_codes : uint
{
  connection,
  message
};

template<typename T> T read(char* &data, size_t &size)
{
  size_t piece_size = sizeof(T);
  assert(size >= piece_size);
  T res = *(T*)data;
  data += piece_size;
  size -= piece_size;
  return res;
}

template<typename T> void write(char* &data, size_t &size, const T& value)
{
  size_t piece_size = sizeof(T);
  *(T*)data = value;
  data += piece_size;
  size += piece_size;
}

void read_char_array(char* &data, size_t &size, char* out, size_t cpy_size)
{
  assert(size >= cpy_size);
  memcpy(out, data, cpy_size);
  data += cpy_size;
  size -= cpy_size;
}

void write_char_array(char* &data, size_t &size, char* in, size_t cpy_size)
{
  memcpy(data, in, cpy_size);
  data += cpy_size;
  size += cpy_size;
}

void parse_message(char* &data, size_t &size)
{
  size_t arr_size = read<size_t>(data, size);
  char message[128];
  read_char_array(data, size, message, arr_size);
  std::cout << message << std::endl;
}

void send_message(char* &data, size_t &size, char* in, size_t cpy_size)
{
  write<net_codes>(data, size, net_codes::message);
  write<size_t>(data, size, cpy_size);
  write_char_array(data, size, in, cpy_size);
}

void parse_by_code(net_codes code, char* &data, size_t &size)
{
  switch (code)
  {
  case net_codes::connection: break;
  case net_codes::message: parse_message(data, size); break;
  }
}

void parse_data(char* data, size_t size)
{
  uint protocol = read<uint>(data, size);
  while (size > 0)
  {
    net_codes code = read<net_codes>(data, size);
    parse_by_code(code, data, size);
  }
}

class packet
{
protected:
  char data[256];
  char* current = nullptr;
  size_t size;

public:
  packet() : current(data) {}
  void copy_from(const packet &other)
  {
    memcpy(data, other.data, 256);
    current = data;
    size = other.size;
  }
};

class writer : public packet
{ 
  template<typename T> void _serialize(T& value)
  {
    size_t piece_size = sizeof(T);
    *(T*)current = value;
    current += piece_size;
    size += piece_size;
  }
public:
  template<typename T> void serialize_s(T& value) { value.serialize(*this); }
  template<typename T> void serialize(T& value) { _serialize(value); }
};

class reader : public packet
{
  template<typename T> void _serialize(T& value)
  {
    size_t piece_size = sizeof(T);
    assert(size >= piece_size);
    value = *(T*)current;
    current += piece_size;
    size -= piece_size;
  }
public:
  template<typename T> void serialize_s(T& value) { value.serialize(*this); }
  template<typename T> void serialize(T& value) { _serialize(value); }
};

struct my_other_struct
{
  template<typename TPacket> void serialize(TPacket &pack) {}
};

struct my_struct
{
  my_struct() {}
  my_struct(int i, const char* name) : i_(i) {}
  int i_ = 5;
  //const char *name_ = "penis";
  my_other_struct s;
  template<typename TPacket> void serialize(TPacket &pack)
  {
    pack.serialize(i_);
    //pack.serialize(name_);
    pack.serialize_s(s);
  }
};

struct net_test
{
  net_test()
  {
    my_struct s(3, "balls");
    writer w;
    w.serialize_s(s);
    reader r;
    r.copy_from(w);
    r.serialize_s(s);
  }
} net_test_;