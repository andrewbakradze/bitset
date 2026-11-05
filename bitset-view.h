#pragma once
#include "bit_logic.h"
#include "bitset-iterator.h"
#include "bitset-word-iterator.h"

#include <algorithm>
#include <bit>
#include <functional>
#include <string>

namespace ct {

class BitSet;

template <typename W>
class BitSetView {
  friend class BitSet;

  template <typename Wt>
  friend class BitSetView;

public:
  using Word = W;
  using Value = bool;

  using Reference = BitSetReference<Word>;
  using ConstReference = BitSetReference<const details::WordType>;

  using Iterator = BitSetIterator<Word>;
  using ConstIterator = BitSetIterator<const details::WordType>;

  using WordIterator = BitSetWordIterator<Word>;
  using ConstWordIterator = BitSetWordIterator<const details::WordType>;

  using View = BitSetView;
  using ConstView = BitSetView<const details::WordType>;

  static constexpr std::size_t NPOS = -1;

  BitSetView()
      : data_(nullptr)
      , begin_(0)
      , end_(0) {}

  template <typename it>
  BitSetView(it begin, it end)
      : data_(begin.data_)
      , begin_(begin.index_)
      , end_(end.index_) {}

  operator ConstView() const {
    return ConstView(data_, begin_, end_);
  }

  Iterator begin() {
    return Iterator(data_, begin_);
  }

  Iterator end() {
    return Iterator(data_, end_);
  }

  ConstIterator begin() const {
    return ConstIterator(data_, begin_);
  }

  ConstIterator end() const {
    return ConstIterator(data_, end_);
  }

  std::size_t size() const {
    return end_ - begin_;
  }

  bool empty() const {
    return size() == 0;
  }

  // if it's ConstView it's automatically return ConstView.
  View subview(std::size_t offset = 0, std::size_t count = NPOS) const {
    if (offset > size()) {
      return View(data_, end_, end_);
    }
    std::size_t length = std::min(count, size() - offset);
    return View(data_, begin_ + offset, begin_ + offset + length);
  }

  // same
  Reference operator[](std::size_t index) const {
    return Reference(data_, global_index(index));
  }

  const View& operator&=(const ConstView& other) const
    requires (!std::is_const_v<Word>)
  {
    if (size() > other.size()) {
      subview(other.size(), size()).reset();
    }
    do_binary_operation(other, std::bit_and<>());
    return *this;
  }

  const View& operator|=(const ConstView& other) const
    requires (!std::is_const_v<Word>)
  {
    do_binary_operation(other, std::bit_or<>());
    return *this;
  }

  const View& operator^=(const ConstView& other) const
    requires (!std::is_const_v<Word>)
  {
    do_binary_operation(other, std::bit_xor<>());
    return *this;
  }

  const View& flip() const
    requires (!std::is_const_v<Word>)
  {
    do_unary_operation(std::bit_not<>());
    return *this;
  }

  const View& set() const
    requires (!std::is_const_v<Word>)
  {
    auto op = [](details::WordType) {
      return ~details::WordType{0};
    };
    do_unary_operation(op);
    return *this;
  }

  const View& reset() const
    requires (!std::is_const_v<Word>)
  {
    auto op = [](details::WordType) {
      return details::WordType{0};
    };
    do_unary_operation(op);
    return *this;
  }

  std::size_t count() const {
    auto op = [](details::WordType word) {
      return static_cast<std::size_t>(std::popcount(word));
    };
    return std::transform_reduce(const_begin_word(), const_end_word(), std::size_t{0}, std::plus<>(), op);
  }

  bool all() const {
    auto op = [](auto word) {
      details::WordType compare_mask =
          word.accessible_bits_ == details::WORD_SIZE ? ~details::WordType{0} : details::mask(0, word.accessible_bits_);
      return word == compare_mask;
    };

    return std::all_of(const_begin_word(), const_end_word(), op);
  }

  bool any() const {
    return std::any_of(const_begin_word(), const_end_word(), [](details::WordType word) {
      return word != 0;
    });
  }

  void swap(View& other) {
    std::swap(end_, other.end_);
    std::swap(begin_, other.begin_);
    std::swap(data_, other.data_);
  }

  friend bool operator==(const ConstView& lhs, const ConstView& rhs);

private:
  BitSetView(Word* data, std::size_t begin, std::size_t end)
      : data_(data)
      , begin_(begin)
      , end_(end) {}

  WordIterator begin_word() const {
    return WordIterator(data_, 0, begin_, end_);
  }

  WordIterator end_word() const {
    return WordIterator(data_, details::size_in_word(size()), begin_, end_);
  }

  ConstWordIterator const_begin_word() const {
    return ConstWordIterator(data_, 0, begin_, end_);
  }

  ConstWordIterator const_end_word() const {
    return ConstWordIterator(data_, details::size_in_word(size()), begin_, end_);
  }

  std::size_t global_index(std::size_t index) const {
    return index + begin_;
  }

  template <typename BitOperation>
  void do_binary_operation(const ConstView& other, BitOperation bit_operation) const {
    if (empty()) {
      return;
    }
    std::size_t comon_words = details::size_in_word(std::min(size(), other.size()));
    std::transform(begin_word(), begin_word() + comon_words, other.begin_word(), begin_word(), bit_operation);
  }

  template <typename Operation>
  void do_unary_operation(Operation word_operation) const {
    if (empty()) {
      return;
    }
    std::transform(begin_word(), end_word(), begin_word(), word_operation);
  }

  static bool is_equal_view(const ConstView& lhs, const ConstView& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin_word(), lhs.end_word(), rhs.begin_word());
  }

  Word* data_;
  std::size_t begin_;
  std::size_t end_;
};

using ConstView = BitSetView<details::WordType>::ConstView;

bool operator!=(const ConstView& lhs, const ConstView& rhs);
BitSet operator&(const ConstView& lhs, const ConstView& rhs);
BitSet operator^(const ConstView& lhs, const ConstView& rhs);
BitSet operator|(const ConstView& lhs, const ConstView& rhs);
BitSet operator~(const ConstView& view);
BitSet operator<<(const ConstView& view, std::size_t count);
BitSet operator>>(const ConstView& view, std::size_t count);

std::string to_string(const ConstView& view);
std::ostream& operator<<(std::ostream& out, const ConstView& view);

} // namespace ct
