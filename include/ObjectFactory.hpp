#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "IMemoryManager.hpp"
#include "GCObject.hpp"

class ObjectFactory
{
public:
    ObjectFactory(IMemoryManager* memoryManager)
      : memoryManager(memoryManager)
    {
    }

    template<typename T, typename... Args>
    T* createObject(Args&&... args);

    template<typename T>
    void destroyObject(T* object);

private:
    IMemoryManager* memoryManager;
};

template<typename T, typename... Args>
T* ObjectFactory::createObject(Args&&... args)
{
    static_assert(std::is_base_of<GCObject, T>::value,
                  "T must be derived from GCObject");

    void* memory = memoryManager->allocate(sizeof(T));
    return new (memory) T(std::forward<Args>(args)...);
}

template<typename T>
void ObjectFactory::destroyObject(T* object)
{
    static_assert(std::is_base_of<GCObject, T>::value,
                  "T must be derived from GCObject");

    object->~T();
    memoryManager->deallocate(static_cast<void*>(object));
}

#endif // OBJECT_FACTORY_H
