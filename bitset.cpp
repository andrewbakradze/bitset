#include "bitset.h"

#include "bit_logic.h"

#include <ostream>

namespace ct {
BitSet::BitSet(std::size_t size)
    : size_(size)
    , data_(empty() ? nullptr : new Word[details::size_in_word(size)]) {}

BitSet::BitSet(std::size_t size, bool value)
    : BitSet(size) {
  if (empty()) {
    return;
  }
  std::fill_n(data_, details::size_in_word(size), value ? ~Word{0} : Word{0});
}

BitSet::BitSet(const BitSet& other)
    : BitSet(other.size()) {
  if (empty()) {
    return;
  }
  std::copy_n(other.data(), details::size_in_word(size()), data());
}

BitSet::BitSet(const ConstView& other)
    : BitSet(other.size()) {
  if (empty()) {
    return;
  }
  std::copy(other.begin_word(), other.end_word(), data());
}

BitSet::BitSet(std::string_view str)
    : BitSet(str.length(), false) {
  if (empty()) {
    return;
  }
  for (std::size_t i = 0; i < size(); ++i) {
    if (str[i] == '1') {
      data()[details::word_index(i)] = details::set_bit(data()[details::word_index(i)], details::index_in_word(i));
    }
  }
}

BitSet::BitSet(ConstIterator start, ConstIterator end)
    : BitSet(ConstView(start, end)) {}

BitSet::~BitSet() {
  delete[] data_;
}

BitSet& BitSet::operator=(const BitSet& other) & {
  if (this == &other) {
    return *this;
  }
  BitSet tmp(other);
  swap(tmp);
  return *this;
}

BitSet& BitSet::operator=(std::string_view str) & {
  BitSet tmp(str);
  swap(tmp);
  return *this;
}

BitSet& BitSet::operator=(const ConstView& other) & {
  BitSet tmp(other);
  swap(tmp);
  return *this;
}

void BitSet::swap(BitSet& other) {
  std::swap(size_, other.size_);
  std::swap(data_, other.data_);
}

std::size_t BitSet::size() const {
  return size_;
}

bool BitSet::empty() const {
  return size_ == 0;
}

BitSet::Reference BitSet::operator[](std::size_t index) {
  return Reference(data(), index);
}

BitSet::ConstReference BitSet::operator[](std::size_t index) const {
  return ConstReference(data(), index);
}

BitSet::Iterator BitSet::begin() {
  return Iterator(data(), 0);
}

BitSet::ConstIterator BitSet::begin() const {
  return ConstIterator(data(), 0);
}

BitSet::Iterator BitSet::end() {
  return Iterator(data(), size());
}

BitSet::ConstIterator BitSet::end() const {
  return ConstIterator(data(), size());
}

BitSet& BitSet::operator&=(const ConstView& other) & {
  View view = View(data(), 0, size());
  view &= other;
  return *this;
}

BitSet& BitSet::operator|=(const ConstView& other) & {
  View view = View(data(), 0, size());
  view |= other;
  return *this;
}

BitSet& BitSet::operator^=(const ConstView& other) & {
  View view = View(data(), 0, size());
  view ^= other;
  return *this;
}

BitSet& BitSet::operator<<=(std::size_t count) & {
  if (details::size_in_word(size() + count) == details::size_in_word(size())) {
    size_ = size() + count;
    subview(size() - count, count).reset();
    return *this;
  }

  BitSet tmp(size() + count, false);
  std::copy_n(data(), details::size_in_word(size()), tmp.data());
  tmp.subview(size(), details::WORD_SIZE - details::index_in_word(size())).reset();
  swap(tmp);
  return *this;
}

BitSet& BitSet::operator>>=(std::size_t count) & {
  std::size_t updated_size = size() > count ? size() - count : 0;
  subview(updated_size, size() - updated_size).reset();
  size_ = updated_size;
  return *this;
}

BitSet& BitSet::flip() & {
  View view = View(data(), 0, size());
  view.flip();
  return *this;
}

BitSet& BitSet::set() & {
  View view = View(data(), 0, size());
  view.set();
  return *this;
}

BitSet& BitSet::reset() & {
  View view = View(data(), 0, size());
  view.reset();
  return *this;
}

bool BitSet::all() const {
  ConstView view = ConstView(data(), 0, size());
  return view.all();
}

bool BitSet::any() const {
  ConstView view = ConstView(data(), 0, size());
  return view.any();
}

std::size_t BitSet::count() const {
  ConstView view = ConstView(data(), 0, size());
  return view.count();
}

BitSet::operator ConstView() const {
  return ConstView(begin(), end());
}

BitSet::operator View() {
  return View(begin(), end());
}

BitSet::View BitSet::subview(std::size_t offset, std::size_t count) {
  if (offset > size()) {
    return View(data(), size(), size());
  }
  std::size_t length = std::min(count, size() - offset);
  return View(data(), offset, length + offset);
}

BitSet::ConstView BitSet::subview(std::size_t offset, std::size_t count) const {
  if (offset > size()) {
    return ConstView(data(), size(), size());
  }
  std::size_t length = std::min(count, size() - offset);
  return ConstView(data(), offset, length + offset);
}

bool operator==(const BitSet& left, const BitSet& right) {
  return BitSet::ConstView(left) == BitSet::ConstView(right);
}

bool operator!=(const BitSet& left, const BitSet& right) {
  return !(left == right);
}

void swap(BitSet& lhs, BitSet& rhs) {
  lhs.swap(rhs);
}

BitSet::Word* BitSet::data() {
  return data_;
}

const BitSet::Word* BitSet::data() const {
  return data_;
}

std::string to_string(const BitSet::ConstView& view) {
  std::string result;
  for (std::size_t i = 0; i < view.size(); ++i) {
    result.push_back(view[i] ? '1' : '0');
  }
  return result;
}

std::ostream& operator<<(std::ostream& out, const BitSet::ConstView& view) {
  for (std::size_t i = 0; i < view.size(); ++i) {
    out << (view[i] ? '1' : '0');
  }
  return out;
}

} // namespace ct
