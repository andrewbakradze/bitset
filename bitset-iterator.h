#pragma once
#include "bitset-reference.h"

#include <compare>
#include <numeric>

namespace ct {

template <typename Word>
class BitSetIterator {
  template <typename w>
  friend class BitSetView;

public:
  using value_type = bool;
  using difference_type = std::ptrdiff_t;
  using reference = BitSetReference<Word>;
  using pointer = void;
  using iterator_category = std::random_access_iterator_tag;

  BitSetIterator() = default;

  BitSetIterator(Word* data, std::size_t index)
      : index_(index)
      , data_(data) {}

  operator BitSetIterator<const Word>() const {
    return BitSetIterator<const Word>{data_, index_};
  }

  reference operator*() const {
    return reference(data_, index_);
  }

  reference operator[](difference_type i) const {
    return *(*this + i);
  }

  BitSetIterator& operator+=(difference_type diff) {
    index_ += diff;
    return *this;
  }

  BitSetIterator& operator-=(difference_type diff) {
    *this += -1 * diff;
    return *this;
  }

  BitSetIterator& operator++() {
    *this += 1;
    return *this;
  }

  BitSetIterator operator++(int) {
    BitSetIterator tmp = *this;
    *this += 1;
    return tmp;
  }

  BitSetIterator& operator--() {
    *this -= 1;
    return *this;
  }

  BitSetIterator operator--(int) {
    BitSetIterator it = *this;
    *this -= 1;
    return it;
  }

  friend BitSetIterator operator+(difference_type diff, const BitSetIterator& it) {
    BitSetIterator result = it;
    result += diff;
    return result;
  }

  friend BitSetIterator operator+(const BitSetIterator& it, difference_type diff) {
    return diff + it;
  }

  friend BitSetIterator operator-(const BitSetIterator& it, difference_type diff) {
    BitSetIterator result = it;
    result -= diff;
    return result;
  }

  friend difference_type operator-(const BitSetIterator& it1, const BitSetIterator& it2) {
    difference_type diff = it1.index_ - it2.index_;
    return diff;
  }

  friend auto operator<=>(const BitSetIterator& lhs, const BitSetIterator& rhs) {
    return lhs.index_ <=> rhs.index_;
  }

  friend bool operator==(const BitSetIterator& lhs, const BitSetIterator& rhs) {
    return std::is_eq(lhs <=> rhs);
  }

private:
  std::size_t index_;
  Word* data_;
};

} // namespace ct
