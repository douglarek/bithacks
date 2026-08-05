#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// True if v is a power of two. Treats 0 as true (classic bit trick pitfall).
template <std::unsigned_integral T>
constexpr bool is_power_of_two_or_zero(T v) noexcept {
    return (v & (v - 1)) == 0;
}

// Correct: power of two and non-zero.
template <std::unsigned_integral T>
constexpr bool is_power_of_two(T v) noexcept {
    return v != 0 && (v & (v - 1)) == 0;
}

// C++20 standard equivalents for comparison in docs/tests:
// std::has_single_bit(v)

} // namespace bithacks
