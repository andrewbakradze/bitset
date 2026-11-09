# BitSet

A bitset implementation in C++20 with bitwise operations, iterators,
and mutable views of bit ranges.

## Build and run

Requires CMake 3.16 or newer and a C++20 compiler.

```sh
cmake -S . -B build
cmake --build build
./build/bitset
```

## Example

```cpp
#include "bitset.h"
#include <iostream>

int main() {
    ct::BitSet bits("101100");
    bits.subview(2, 3).flip();
    std::cout << bits << '\n'; // 100010
}
```

`subview(offset, count)` refers to bits in the original set, so changes to the
view modify the original.

See `main.cpp` for more examples.
