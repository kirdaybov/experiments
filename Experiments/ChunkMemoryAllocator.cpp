#include "ChunkMemoryAllocator.h"

CMemoryAllocator* CMemoryAllocator::Instance = new CMemoryAllocator();
const size_t CMemoryAllocator::ChunkSize = 64;
const size_t CMemoryAllocator::ChunkNumber = 200;