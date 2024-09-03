#include "GCObject.hpp"

GCObject::GCObject(MemoryPool* pool)
  : marked(false)
  , deallocated(false)
  , memoryPool(pool)
{
    std::cout << "GCObject created: " << this << std::endl;
}

GCObject::~GCObject()
{
    std::cout << "GCObject destroyed: " << this << std::endl;
}

void GCObject::mark()
{
    if (!marked.exchange(true)) {
        std::cout << "GCObject marked: " << this << std::endl;
        traverse();
    }
}

void GCObject::unmark()
{
    marked.store(false);
    std::cout << "GCObject unmarked: " << this << std::endl;
}

bool GCObject::isMarked() const
{
    return marked.load();
}

bool GCObject::isDeallocated() const
{
    return deallocated.load();
}

void GCObject::deallocate()
{
    if (!deallocated.exchange(true)) { // Check and set the deallocated flag
        if (memoryPool) {
            memoryPool->deallocate(this); // Return memory to the pool
        } else {
            delete this; // Fallback if no memory pool is set
        }
    }
}
