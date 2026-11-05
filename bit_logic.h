#pragma once
#include <cstddef>
#include <cstdint>

namespace ct::details {

using WordType = uint64_t;

static constexpr std::size_t WORD_SIZE = sizeof(WordType) * 8;

std::size_t size_in_word(std::size_t size);

std::size_t index_in_word(std::size_t index);

std::size_t word_index(std::size_t index);

WordType bit(const WordType& word, std::size_t index);

WordType set_bit(const WordType& word, std::size_t index);

WordType reset_bit(const WordType& word, std::size_t index);

WordType flip_bit(const WordType& word, std::size_t index);

WordType mask(std::size_t left_border, std::size_t right_border);

} // namespace ct::details
