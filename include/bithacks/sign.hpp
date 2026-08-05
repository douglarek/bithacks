#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// sign_neg01: -1 if v < 0, else 0.
template <std::signed_integral T> constexpr T sign_neg01(T v) noexcept {
    return static_cast<T>(-(v < 0));
}

// Faster but relies on arithmetic right shift (implementation-defined in C;
// in practice arithmetic on two's complement platforms).
template <std::signed_integral T> constexpr T sign_neg01_shift(T v) noexcept {
    return static_cast<T>(v >> (bit_width_v<T> - 1));
}

// Portable via unsigned shift of the bit pattern.
template <std::signed_integral T>
constexpr T sign_neg01_unsigned_shift(T v) noexcept {
    using U = std::make_unsigned_t<T>;
    return -static_cast<T>(static_cast<U>(v) >> (bit_width_v<T> - 1));
}

// sign_pm1: -1 if v < 0, else +1. (zero maps to +1)
template <std::signed_integral T> constexpr T sign_pm1(T v) noexcept {
    return static_cast<T>(1) | sign_neg01_shift(v);
}

// sign_neg_zero_pos: -1, 0, or +1.
template <std::signed_integral T> constexpr T sign_neg_zero_pos(T v) noexcept {
    // Portable, branchless, and usually compiles well:
    return static_cast<T>((v > 0) - (v < 0));
}

template <std::signed_integral T>
constexpr T sign_neg_zero_pos_shift(T v) noexcept {
    return static_cast<T>((v != 0) | (v >> (bit_width_v<T> - 1)));
}

// Non-negative indicator: +1 if v >= 0, else 0.
template <std::signed_integral T> constexpr T sign_nonneg01(T v) noexcept {
    using U = std::make_unsigned_t<T>;
    return static_cast<T>(1) ^
           static_cast<T>(static_cast<U>(v) >> (bit_width_v<T> - 1));
}

// Opposite signs: true iff x and y have different sign bits.
template <std::signed_integral T>
constexpr bool opposite_signs(T x, T y) noexcept {
    return (x ^ y) < 0;
}

} // namespace bithacks
