#include "bitset.h"

#include <iostream>

int main() {
  ct::BitSet bits("101100");
  const ct::BitSet mask("110010");

  std::cout << "Bits: " << bits << '\n';
  std::cout << "AND: " << (bits & mask) << '\n';
  std::cout << "OR: " << (bits | mask) << '\n';
  std::cout << "XOR: " << (bits ^ mask) << '\n';

  bits[1] = true;
  bits.subview(2, 3).flip();
  std::cout << "Modified: " << bits << '\n';
}
