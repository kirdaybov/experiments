#pragma once
#include <cassert>

typedef unsigned int uint;

const uint protocol_id = 0x1;

enum class net_codes : uint
{
	connection
};

template<typename T> T read(char* &data, size_t &size)
{
  size_t piece_size = sizeof(uint);
  assert(size >= piece_size);
	T res = *(T*)data;
	data += piece_size;
	size -= piece_size;
  return res;
}

void parse_by_code(net_codes code, char* &data, size_t &size)
{
  switch (code)
  {
    case net_codes::connection:;
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