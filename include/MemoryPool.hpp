#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "IMemoryManager.hpp"
#include <vector>
#include <mutex>

class MemoryPool : public IMemoryManager
{
public:
    MemoryPool(size_t blockSize, size_t blockCount);
    ~MemoryPool() override;

    void* allocate(size_t size) override;
    void deallocate(void* block) override;

private:
    size_t blockSize;
    size_t blockCount;
    std::vector<char> pool;
    std::vector<void*> freeBlocks;
    std::mutex mtx; // Mutex for thread safety
};

#endif // MEMORY_POOL_H
