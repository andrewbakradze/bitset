//
// Created by Андрей Бакрадзе on 19.04.2026.
//

#include "bitset-view.h"

#include "bitset.h"

namespace ct {

bool operator==(const BitSet::ConstView& lhs, const BitSet::ConstView& rhs) {
  return BitSet::ConstView::is_equal_view(lhs, rhs);
}

bool operator!=(const BitSet::ConstView& lhs, const BitSet::ConstView& rhs) {
  return !(lhs == rhs);
}

BitSet operator&(const BitSet::ConstView& lhs, const BitSet::ConstView& rhs) {
  BitSet result(lhs);
  return result &= rhs;
}

BitSet operator|(const BitSet::ConstView& lhs, const BitSet::ConstView& rhs) {
  BitSet result(lhs);
  return result |= rhs;
}

BitSet operator^(const BitSet::ConstView& lhs, const BitSet::ConstView& rhs) {
  BitSet result(lhs);
  return result ^= rhs;
}

BitSet operator~(const BitSet::ConstView& view) {
  BitSet result(view);
  return result.flip();
}

BitSet operator<<(const BitSet::ConstView& view, std::size_t count) {
  BitSet result(view);
  return result <<= count;
}

BitSet operator>>(const BitSet::ConstView& view, std::size_t count) {
  BitSet result(view);
  return result >>= count;
}

void swap(BitSet::ConstView& lhs, BitSet::ConstView& rhs) {
  lhs.swap(rhs);
}

void swap(BitSet::View& lhs, BitSet::View& rhs) {
  lhs.swap(rhs);
}

} // namespace ct
