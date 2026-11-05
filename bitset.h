#pragma once

#include "bitset-iterator.h"
#include "bitset-reference.h"
#include "bitset-view.h"

#include <cstddef>
#include <string_view>

namespace ct {

class BitSet {
public:
  using Value = bool;
  using Word = uint64_t;

  using Iterator = BitSetIterator<Word>;
  using ConstIterator = BitSetIterator<const Word>;

  using Reference = BitSetReference<Word>;
  using ConstReference = BitSetReference<const Word>;

  using View = BitSetView<Word>;
  using ConstView = BitSetView<const Word>;

  static constexpr std::size_t NPOS = -1;

  BitSet()
      : size_(0)
      , data_(nullptr) {}

  BitSet(std::size_t size, bool value);
  BitSet(const BitSet& other);
  BitSet(ConstIterator start, ConstIterator end);
  explicit BitSet(std::string_view str);
  explicit BitSet(const ConstView& other);

  BitSet& operator=(const BitSet& other) &;
  BitSet& operator=(std::string_view str) &;
  BitSet& operator=(const ConstView& other) &;

  ~BitSet();

  void swap(BitSet& other);

  std::size_t size() const;
  bool empty() const;

  Reference operator[](std::size_t index);
  ConstReference operator[](std::size_t index) const;

  Iterator begin();
  ConstIterator begin() const;
  Iterator end();
  ConstIterator end() const;

  BitSet& operator&=(const ConstView& other) &;
  BitSet& operator|=(const ConstView& other) &;
  BitSet& operator^=(const ConstView& other) &;
  BitSet& operator<<=(std::size_t count) &;
  BitSet& operator>>=(std::size_t count) &;

  BitSet& flip() &;
  BitSet& set() &;
  BitSet& reset() &;

  bool all() const;
  bool any() const;
  std::size_t count() const;

  operator ConstView() const;
  operator View();

  View subview(std::size_t offset = 0, std::size_t count = NPOS);
  ConstView subview(std::size_t offset = 0, std::size_t count = NPOS) const;

private:
  BitSet(std::size_t size);

  Word* data();
  const Word* data() const;

  std::size_t size_;
  Word* data_;
};

bool operator==(const BitSet& left, const BitSet& right);
bool operator!=(const BitSet& left, const BitSet& right);

// BitSet operator_(const BitSet& lhs, const BitSet& rhs);
// они не нужны так как у нас есть конверсия к view и операторы для view, я уточнял мне вроде сказали что если работает
// то их можно не делать

void swap(BitSet& lhs, BitSet& rhs);
void swap(BitSet::ConstView& lhs, BitSet::ConstView& rhs);
void swap(BitSet::View& lhs, BitSet::View& rhs);

} // namespace ct
