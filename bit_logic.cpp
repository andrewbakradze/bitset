//
// Created by Андрей Бакрадзе on 12.04.2026.

#include "bit_logic.h"

//
namespace ct::details {

std::size_t size_in_word(std::size_t size) {
  return (size + WORD_SIZE - 1) / WORD_SIZE;
}

std::size_t index_in_word(std::size_t index) {
  return index % WORD_SIZE;
}

std::size_t word_index(std::size_t index) {
  return index / WORD_SIZE;
}

WordType bit(const WordType& word, std::size_t index) {
  return word & (WordType{1} << index);
}

WordType set_bit(const WordType& word, std::size_t index) {
  return word | bit(~word, index);
}

WordType reset_bit(const WordType& word, std::size_t index) {
  return word ^ bit(word, index);
}

WordType flip_bit(const WordType& word, std::size_t index) {
  return reset_bit(word, index) | bit(~word, index);
}

// bit mask [l:r)
WordType mask(std::size_t left_border, std::size_t right_border) {
  WordType left_part = ~WordType{0} << left_border;
  if (right_border == 0) {
    return left_part;
  }
  WordType right_part = ~WordType{0} >> (WORD_SIZE - right_border);
  return left_part & right_part;
}

} // namespace ct::details
