#pragma once

#include <cassert>

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