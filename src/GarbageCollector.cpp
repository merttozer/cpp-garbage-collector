#include "GarbageCollector.hpp"
#include <iostream>

GarbageCollector& GarbageCollector::getInstance()
{
    static GarbageCollector instance;
    return instance;
}

void GarbageCollector::registerObject(GCObject* object)
{
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety
    objects.insert(object);
    std::cout << "Object registered: " << object << std::endl;
}

void GarbageCollector::unregisterObject(GCObject* object)
{
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety
    objects.erase(object);
    std::cout << "Object unregistered: " << object << std::endl;
}

void GarbageCollector::collect()
{
    if (collecting.exchange(true)) {
        // Already collecting, prevent reentrant calls
        return;
    }

    std::cout << "Garbage collection started..." << std::endl;
    mark();
    sweep();
    std::cout << "Garbage collection completed." << std::endl;

    collecting.store(false);
}

void GarbageCollector::setMarkingStrategy(
  const std::function<void(GCObject*)>& strategy)
{
    markingStrategy = strategy;
}

void GarbageCollector::mark()
{
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety
    if (markingStrategy) {
        for (auto object : objects) {
            markingStrategy(object);
        }
    } else {
        for (auto object : objects) {
            if (!object->isMarked()) {
                object->mark();
            }
        }
    }
}

void GarbageCollector::sweep()
{
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety
    for (auto it = objects.begin(); it != objects.end();) {
        if (!(*it)->isMarked()) {
            (*it)->deallocate(); // Use deallocate instead of delete
            it = objects.erase(it);
            std::cout << "Unreachable object deleted." << std::endl;
        } else {
            (*it)->unmark(); // Reset the mark for the next collection cycle
            ++it;
        }
    }
}
