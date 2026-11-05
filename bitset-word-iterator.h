
#pragma once
#include "bitset-word-reference.h"

#include <algorithm>
#include <compare>

namespace ct {

template <typename Word>
class BitSetWordIterator {
  template <typename w>
  friend class BitSetView;

public:
  using value_type = details::WordType;
  using difference_type = std::ptrdiff_t;
  using reference = BitSetWordReference<Word>;
  using pointer = void;
  using iterator_category = std::random_access_iterator_tag;

  BitSetWordIterator() = default;

  operator BitSetWordIterator<const Word>() const {
    return BitSetWordIterator<const Word>{data_, word_index_, start_index_, end_index_};
  }

  reference operator*() const {
    std::size_t accessible_bits =
        std::min(details::WORD_SIZE, end_index_ - (start_index_ + (word_index_ * details::WORD_SIZE)));
    return reference(data_, start_index_ + (word_index_ * details::WORD_SIZE), accessible_bits);
  }

  reference operator[](difference_type i) const {
    return *(*this + i);
  }

  BitSetWordIterator& operator+=(difference_type diff) {
    word_index_ += diff;
    return *this;
  }

  BitSetWordIterator& operator-=(difference_type diff) {
    *this += -1 * diff;
    return *this;
  }

  BitSetWordIterator& operator++() {
    *this += 1;
    return *this;
  }

  BitSetWordIterator operator++(int) {
    BitSetWordIterator tmp = *this;
    *this += 1;
    return tmp;
  }

  BitSetWordIterator& operator--() {
    *this -= 1;
    return *this;
  }

  BitSetWordIterator operator--(int) {
    BitSetWordIterator it = *this;
    *this -= 1;
    return it;
  }

  friend BitSetWordIterator operator+(difference_type diff, const BitSetWordIterator& it) {
    BitSetWordIterator result = it;
    result += diff;
    return result;
  }

  friend BitSetWordIterator operator+(const BitSetWordIterator& it, difference_type diff) {
    return diff + it;
  }

  friend BitSetWordIterator operator-(const BitSetWordIterator& it, difference_type diff) {
    BitSetWordIterator result = it;
    result -= diff;
    return result;
  }

  friend difference_type operator-(const BitSetWordIterator& it1, const BitSetWordIterator& it2) {
    difference_type diff = it1.word_index_ - it2.word_index_;
    return diff;
  }

  friend auto operator<=>(const BitSetWordIterator& lhs, const BitSetWordIterator& rhs) {
    return lhs.word_index_ <=> rhs.word_index_;
  }

  friend bool operator==(const BitSetWordIterator& lhs, const BitSetWordIterator& rhs) {
    return std::is_eq(lhs <=> rhs);
  }

private:
  BitSetWordIterator(Word* data, std::size_t word_index, std::size_t start_index, std::size_t end_index)
      : word_index_(word_index)
      , start_index_(start_index)
      , end_index_(end_index)
      , data_(data) {}

  std::size_t word_index_;
  std::size_t start_index_;
  std::size_t end_index_;
  Word* data_;
};

} // namespace ct
