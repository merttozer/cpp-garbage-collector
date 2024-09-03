#ifndef IMEMORY_MANAGER_H
#define IMEMORY_MANAGER_H

#include <cstddef> // include this for size_t in C++

class IMemoryManager
{
public:
    virtual ~IMemoryManager() = default;
    virtual void* allocate(size_t size) = 0;
    virtual void deallocate(void* block) = 0;
};

#endif // IMEMORY_MANAGER_H
