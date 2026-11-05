#pragma once
#include "bit_logic.h"

#include <cstddef>
#include <type_traits>

namespace ct {
template <typename Word>
class BitSetReference {
  friend class BitSet;

  template <typename w>
  friend class BitSetView;

  template <typename w>
  friend class BitSetReference;

  template <typename w>
  friend class BitSetIterator;

  BitSetReference(Word* data, std::size_t bit_index)
      : data_(&data[details::word_index(bit_index)])
      , bit_index_(details::index_in_word(bit_index)) {}

public:
  operator BitSetReference<const Word>() const {
    return BitSetReference<const Word>(data_, bit_index_);
  }

  operator bool() const {
    return details::bit(*data_, bit_index_);
  }

  BitSetReference& operator=(bool value)
    requires (!std::is_const_v<Word>)
  {
    *data_ = value ? details::set_bit(*data_, bit_index_) : details::reset_bit(*data_, bit_index_);
    return *this;
  }

  void flip() const
    requires (!std::is_const_v<Word>)
  {
    *data_ = details::flip_bit(*data_, bit_index_);
  }

private:
  Word* data_;
  std::size_t bit_index_;
};

} // namespace ct
