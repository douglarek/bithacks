#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// Branchless min/max using comparison + XOR.
// r = y ^ ((x ^ y) & -(x < y))  => min
// r = x ^ ((x ^ y) & -(x < y))  => max
template <std::signed_integral T>
constexpr T min_branchless(T x, T y) noexcept {
    return y ^ ((x ^ y) & static_cast<T>(-(x < y)));
}

template <std::signed_integral T>
constexpr T max_branchless(T x, T y) noexcept {
    return x ^ ((x ^ y) & static_cast<T>(-(x < y)));
}

// Quick-and-dirty form (requires no overflow in x - y).
// Prefer the XOR form above unless you control the value range.
template <std::signed_integral T> constexpr T min_quick(T x, T y) noexcept {
    const T d = x - y;
    return y + (d & static_cast<T>(d >> (bit_width_v<T> - 1)));
}

template <std::signed_integral T> constexpr T max_quick(T x, T y) noexcept {
    const T d = x - y;
    return x - (d & static_cast<T>(d >> (bit_width_v<T> - 1)));
}

// Unsigned overloads (comparison-based, fully portable).
template <std::unsigned_integral T>
constexpr T min_branchless(T x, T y) noexcept {
    return y ^ ((x ^ y) & bool_mask<T>(x < y));
}

template <std::unsigned_integral T>
constexpr T max_branchless(T x, T y) noexcept {
    return x ^ ((x ^ y) & bool_mask<T>(x < y));
}

} // namespace bithacks
