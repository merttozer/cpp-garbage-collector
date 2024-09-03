#ifndef GC_POINTER_H
#define GC_POINTER_H

#include "GCObject.hpp"
#include "MemoryPool.hpp"
#include "GarbageCollector.hpp"
#include <utility> // For std::exchange
#include <atomic>

template<typename T>
class GCPointer
{
public:
    GCPointer();
    GCPointer(T* rawPtr, MemoryPool* pool);
    GCPointer(const GCPointer& other);
    GCPointer(GCPointer&& other) noexcept;

    GCPointer& operator=(const GCPointer& other);
    GCPointer& operator=(GCPointer&& other) noexcept;
    ~GCPointer();

    T& operator*() const;
    T* operator->() const;
    T* get() const;

    explicit operator bool() const;

private:
    T* ptr;
    std::atomic<int>* refCount;
    MemoryPool* memoryPool;

    void release(bool manualUnregister = true);
    void acquire(T* rawPtr, MemoryPool* pool);
};

// Default constructor
template<typename T>
GCPointer<T>::GCPointer()
  : ptr(nullptr)
  , refCount(nullptr)
  , memoryPool(nullptr)
{
}

// Constructor with raw pointer and memory pool
template<typename T>
GCPointer<T>::GCPointer(T* rawPtr, MemoryPool* pool)
  : ptr(rawPtr)
  , refCount(new std::atomic<int>(1))
  , memoryPool(pool)
{
    if (ptr) {
        GarbageCollector::getInstance().registerObject(ptr);
        // ptr->mark(); // Optionally mark the object immediately if necessary
    }
}

// Copy constructor
template<typename T>
GCPointer<T>::GCPointer(const GCPointer& other)
  : ptr(other.ptr)
  , refCount(other.refCount)
  , memoryPool(other.memoryPool)
{
    if (ptr) {
        ++(*refCount);
        ptr->mark();
    }
}

// Move constructor
template<typename T>
GCPointer<T>::GCPointer(GCPointer&& other) noexcept
  : ptr(std::exchange(other.ptr, nullptr))
  , refCount(std::exchange(other.refCount, nullptr))
  , memoryPool(std::exchange(other.memoryPool, nullptr))
{
}

// Copy assignment operator
template<typename T>
GCPointer<T>& GCPointer<T>::operator=(const GCPointer& other)
{
    if (this != &other) {
        release();
        ptr = other.ptr;
        refCount = other.refCount;
        memoryPool = other.memoryPool;
        if (ptr) {
            ++(*refCount);
            ptr->mark();
        }
    }
    return *this;
}

// Move assignment operator
template<typename T>
GCPointer<T>& GCPointer<T>::operator=(GCPointer&& other) noexcept
{
    if (this != &other) {
        release();
        ptr = std::exchange(other.ptr, nullptr);
        refCount = std::exchange(other.refCount, nullptr);
        memoryPool = std::exchange(other.memoryPool, nullptr);
    }
    return *this;
}

// Destructor
template<typename T>
GCPointer<T>::~GCPointer()
{
    release();
}

// Dereference operator
template<typename T>
T& GCPointer<T>::operator*() const
{
    return *ptr;
}

// Member access operator
template<typename T>
T* GCPointer<T>::operator->() const
{
    return ptr;
}

// Get the raw pointer
template<typename T>
T* GCPointer<T>::get() const
{
    return ptr;
}

// Bool conversion operator
template<typename T>
GCPointer<T>::operator bool() const
{
    return ptr != nullptr;
}

template<typename T>
void GCPointer<T>::release(bool manualUnregister)
{
    if (ptr && --(*refCount) == 0) {
        delete refCount;

        // Deallocate memory only if the object hasn't been deallocated yet
        if (!ptr->isDeallocated()) {
            if (memoryPool) {
                memoryPool->deallocate(ptr); // Return memory to the pool
            } else {
                delete ptr; // Fallback if no memory pool is set
            }
        }

        // Unregister the object from the garbage collector if required
        if (manualUnregister) {
            GarbageCollector::getInstance().unregisterObject(ptr);
        }
    }

    // Reset pointers to ensure no further operations are performed on invalid
    // memory
    ptr = nullptr;
    refCount = nullptr;
    memoryPool = nullptr;
}

// Acquire method for initializing the pointer and its management
template<typename T>
void GCPointer<T>::acquire(T* rawPtr, MemoryPool* pool)
{
    ptr = rawPtr;
    refCount = new std::atomic<int>(1);
    memoryPool = pool;
    if (ptr) {
        GarbageCollector::getInstance().registerObject(ptr);
        // ptr->mark();
    }
}

#endif // GC_POINTER_H
