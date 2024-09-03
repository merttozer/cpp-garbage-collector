#ifndef GC_OBJECT_H
#define GC_OBJECT_H

#include <atomic>
#include <iostream>
#include "MemoryPool.hpp"

class GCObject
{
public:
    GCObject(MemoryPool* pool = nullptr);
    virtual ~GCObject();

    void mark();
    void unmark();
    bool isMarked() const;
    bool isDeallocated() const; // New method to check deallocation status

    virtual void traverse() = 0;

    void deallocate(); // New method to return the object to the pool

private:
    std::atomic<bool> marked;
    std::atomic<bool> deallocated; // Flag to track deallocation status
    MemoryPool* memoryPool;        // Reference to the memory pool
};

#endif // GC_OBJECT_H
