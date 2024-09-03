#include "MemoryPool.hpp"
#include <stdexcept>
#include <iostream>

MemoryPool::MemoryPool(size_t blockSize, size_t blockCount)
  : blockSize(blockSize)
  , blockCount(blockCount)
  , pool(blockSize * blockCount)
{

    freeBlocks.reserve(blockCount);
    for (size_t i = 0; i < blockCount; ++i) {
        freeBlocks.push_back(pool.data() + i * blockSize);
    }

    std::cout << "MemoryPool created with block size " << blockSize
              << " and block count " << blockCount << "." << std::endl;
}

MemoryPool::~MemoryPool()
{
    // Destructor logic, if necessary, can be expanded here
    std::cout << "MemoryPool destroyed." << std::endl;
}

void* MemoryPool::allocate(size_t size)
{
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety

    if (size > blockSize) {
        throw std::invalid_argument("Requested size exceeds block size");
    }

    if (freeBlocks.empty()) {
        throw std::bad_alloc();
    }

    void* block = freeBlocks.back();
    freeBlocks.pop_back();

    std::cout << "Allocated memory block at address " << block << "."
              << std::endl;

    return block;
}

void MemoryPool::deallocate(void* block)
{
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety

    freeBlocks.push_back(block);

    std::cout << "Deallocated memory block at address " << block << "."
              << std::endl;
}
