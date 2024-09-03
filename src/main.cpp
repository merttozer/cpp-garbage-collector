#include <iostream>
#include "MemoryPool.hpp"
#include "ObjectFactory.hpp"
#include "GCPointer.hpp"
#include "GarbageCollector.hpp"

class TestObject : public GCObject
{
public:
    TestObject(int value, MemoryPool* pool)
      : GCObject(pool)
      , value(value)
    {
        std::cout << "TestObject created with value: " << value << std::endl;
    }

    ~TestObject()
    {
        std::cout << "TestObject with value " << value << " destroyed."
                  << std::endl;
    }

    int getValue() const { return value; }

    void traverse() override
    {
        std::cout << "Traversing TestObject with value: " << value << std::endl;
        // Here you could mark related objects if any
    }

private:
    int value;
};

int main()
{
    size_t blockSize = sizeof(TestObject);
    size_t blockCount = 10;

    MemoryPool pool(blockSize, blockCount);
    ObjectFactory factory(&pool);

    {
        TestObject* rawPtr1 = factory.createObject<TestObject>(42, &pool);
        GCPointer<TestObject> ptr1(rawPtr1, &pool);

        TestObject* rawPtr2 = factory.createObject<TestObject>(84, &pool);
        GCPointer<TestObject> ptr2(rawPtr2, &pool);

        TestObject* rawPtr3 = factory.createObject<TestObject>(15, &pool);
        GCPointer<TestObject> ptr3(rawPtr3, &pool);

        std::cout << "Object 1 value: " << ptr1->getValue() << std::endl;
        std::cout << "Object 2 value: " << ptr2->getValue() << std::endl;
        std::cout << "Object 3 value: " << ptr3->getValue() << std::endl;

        auto evenMarkingStrategy = [](GCObject* object) {
            TestObject* testObj = dynamic_cast<TestObject*>(object);
            if (testObj && (testObj->getValue() % 2 == 0)) {
                testObj->mark();
            }
        };

        GarbageCollector::getInstance().setMarkingStrategy(evenMarkingStrategy);
    }

    std::cout << "Collecting garbage with custom strategy..." << std::endl;
    GarbageCollector::getInstance().collect();

    return 0;
}
