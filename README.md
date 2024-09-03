# C++ Garbage Collector

This project implements a custom garbage collector in C++ with a focus on memory management and automatic object lifetime management. The garbage collector is designed to work with a `MemoryPool` and supports custom marking strategies using the Strategy pattern.

## Features

- **Memory Pool Management**: Efficient memory allocation and deallocation using a custom memory pool.
- **Garbage Collection**: Automatic garbage collection with support for custom marking strategies.
- **Reference Counting**: Automatic management of object lifetimes through reference counting.
- **Thread Safety**: All memory and garbage collection operations are thread-safe.

## Components

### 1. `MemoryPool`

A memory pool class that handles the allocation and deallocation of fixed-size memory blocks. The pool is thread-safe and designed to work efficiently with the garbage collector.

### 2. `GCObject`

A base class for objects that can be garbage collected. It provides the necessary interface for marking objects and supports thread-safe operations.

### 3. `GCPointer`

A smart pointer class that manages the lifetime of `GCObject` instances. It automatically registers and unregisters objects with the `GarbageCollector`.

### 4. `GarbageCollector`

The main garbage collector class that handles the marking and sweeping of objects. It supports custom marking strategies, allowing for flexible garbage collection behavior.

### 5. `ObjectFactory`

A factory class that manages the creation and destruction of `GCObject` instances using a custom memory manager.

## Getting Started

### Prerequisites

- C++17 or later
- CMake
- A C++ compiler (e.g., GCC, Clang, MSVC)

### Building the Project

1. Clone the repository:

    ```bash
    git clone https://github.com/yourusername/garbage-collector.git
    cd garbage-collector
    ```

2. Create a build directory and navigate to it:

    ```bash
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:

    ```bash
    cmake ..
    ```

4. Build the project using Make or your preferred build tool:

    ```bash
    make
    ```

5. Run the executable:

    ```bash
    ./garbage_collector
    ```

### Example Usage

The `main.cpp` file provides an example of how to use the garbage collector with a custom marking strategy. The example creates several objects, applies a custom marking strategy, and then triggers garbage collection.

```cpp
#include "MemoryPool.hpp"
#include "ObjectFactory.hpp"
#include "GCPointer.hpp"
#include "GarbageCollector.hpp"

int main() {
    // Memory pool and object factory setup
    MemoryPool pool(sizeof(TestObject), 10);
    ObjectFactory factory(&pool);

    // Create objects and use GCPointer to manage them
    GCPointer<TestObject> ptr1(factory.createObject<TestObject>(42));
    GCPointer<TestObject> ptr2(factory.createObject<TestObject>(84));
    GCPointer<TestObject> ptr3(factory.createObject<TestObject>(15));

    // Custom marking strategy: Mark only objects with even values
    auto evenMarkingStrategy = [](GCObject* object) {
        TestObject* testObj = dynamic_cast<TestObject*>(object);
        if (testObj && (testObj->getValue() % 2 == 0)) {
            testObj->mark();
        }
    };

    GarbageCollector::getInstance().setMarkingStrategy(evenMarkingStrategy);

    // Trigger garbage collection
    GarbageCollector::getInstance().collect();

    return 0;
}
```

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the LICENSE file for details.