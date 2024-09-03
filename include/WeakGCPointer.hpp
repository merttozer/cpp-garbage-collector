#ifndef WEAK_GC_POINTER_H
#define WEAK_GC_POINTER_H

#include "GCObject.hpp"
#include "GCPointer.hpp"

template<typename T>
class WeakGCPointer
{
public:
    WeakGCPointer();
    WeakGCPointer(const GCPointer<T>& strongPtr);

    T* lock() const;
    bool expired() const;

private:
    T* ptr;
};

template<typename T>
WeakGCPointer<T>::WeakGCPointer()
  : ptr(nullptr)
{
}

template<typename T>
WeakGCPointer<T>::WeakGCPointer(const GCPointer<T>& strongPtr)
  : ptr(strongPtr.get())
{
}

template<typename T>
T* WeakGCPointer<T>::lock() const
{
    if (ptr && ptr->isMarked()) {
        return ptr;
    }
    return nullptr;
}

template<typename T>
bool WeakGCPointer<T>::expired() const
{
    return !ptr || !ptr->isMarked();
}

#endif // WEAK_GC_POINTER_H
