#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include "GCObject.hpp"
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <functional>

class GarbageCollector
{
public:
    static GarbageCollector& getInstance();

    void registerObject(GCObject* object);
    void unregisterObject(GCObject* object);
    void collect();

    GarbageCollector(GarbageCollector& other) = delete;
    void operator=(const GarbageCollector&) = delete;

    void setMarkingStrategy(const std::function<void(GCObject*)>& strategy);

private:
    GarbageCollector() = default;

    void mark();
    void sweep();

    std::unordered_set<GCObject*> objects;
    std::mutex mtx;
    std::atomic<bool> collecting = false;
    std::function<void(GCObject*)> markingStrategy;
};

#endif // GARBAGE_COLLECTOR_H
