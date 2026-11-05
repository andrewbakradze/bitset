#pragma once
#include "bit_logic.h"

#include <cstddef>
#include <type_traits>

namespace ct {
template <typename Word>
class BitSetWordReference {
  template <typename w>
  friend class BitSetWordIterator;

  template <typename w>
  friend class BitSetView;

  BitSetWordReference(Word* data, std::size_t index, std::size_t accessible_bits)
      // accessible_bits available for read and write in view
      : data_(data)
      , start_index_(index)
      , accessible_bits_(accessible_bits) {}

public:
  operator BitSetWordReference<const Word>() const {
    return BitSetWordReference<const Word>(data_, start_index_, accessible_bits_);
  }

  operator details::WordType() const { // for read
    return make_word(start_index_);
  }

  BitSetWordReference& operator=(details::WordType value)
    requires (!std::is_const_v<Word>)
  {
    write_value_in_word(value);
    return *this;
  }

private:
  Word make_start(std::size_t index) const {
    Word result = data_[details::word_index(index)];
    return result >> details::index_in_word(index); // get [index, end_word] bits
  }

  bool is_need_end(std::size_t index) const {
    return details::index_in_word(index) != 0 && accessible_bits_ > details::WORD_SIZE - details::index_in_word(index);
  }

  Word make_end(std::size_t index) const {
    if (!is_need_end(index)) {
      return 0;
    }
    Word result = data_[details::word_index(index) + 1];
    return result << (details::WORD_SIZE - details::index_in_word(index)); // get [0, WORD_SIZE - index] bits
    //(same amount as was discarded in make_start)
  }

  Word make_word(std::size_t index) const {
    if (accessible_bits_ == 0) {
      return 0;
    }
    Word result = make_start(index) | make_end(index);
    if (accessible_bits_ >= details::WORD_SIZE) {
      return result;
    }
    return result & details::mask(0, accessible_bits_); // delete inaccessible bits for view
  }

  std::size_t start_bits_count() const {
    std::size_t bit_in_start_word = details::WORD_SIZE - details::index_in_word(start_index_);
    return bit_in_start_word > accessible_bits_ ? accessible_bits_ : bit_in_start_word;
  }

  void write_in_start_word(Word value)
    requires (!std::is_const_v<Word>)
  {
    std::size_t start_index_in_start_word = details::index_in_word(start_index_);
    details::WordType start_mask =
        details::mask(start_index_in_start_word, start_index_in_start_word + start_bits_count());

    data_[details::word_index(start_index_)] =
        (data_[details::word_index(start_index_)] & ~start_mask) | ((value << start_index_in_start_word) & start_mask);
  }

  void write_in_end_word(Word value)
    requires (!std::is_const_v<Word>)
  {
    if (!is_need_end(start_index_)) {
      return;
    }
    std::size_t end_bits = accessible_bits_ - start_bits_count();
    details::WordType end_mask = details::mask(0, end_bits);
    data_[details::word_index(start_index_) + 1] =
        (data_[details::word_index(start_index_) + 1] & ~end_mask) | ((value >> start_bits_count()) & end_mask);
  }

  void write_value_in_word(Word value)
    requires (!std::is_const_v<Word>)
  {
    if (accessible_bits_ == 0) {
      return;
    }
    write_in_start_word(value);
    write_in_end_word(value);
  }

  Word* data_;
  std::size_t start_index_;
  std::size_t accessible_bits_;
};

} // namespace ct
