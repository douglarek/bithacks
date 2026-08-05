#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// Absolute value without branching.
// Returns unsigned to avoid the INT_MIN overflow problem of signed abs.
// For two's complement: |INT_MIN| cannot be represented as signed int.
template <std::signed_integral T>
constexpr std::make_unsigned_t<T> abs_unsigned(T v) noexcept {
    using U = std::make_unsigned_t<T>;
    // Portable mask: 0 or all-ones as unsigned, then do arithmetic in U
    // so INT_MIN does not trigger signed overflow warnings/UB.
    const U mask = static_cast<U>(-static_cast<U>(v < 0));
    const U uv = static_cast<U>(v);
    return (uv + mask) ^ mask;
}

// Classic form using arithmetic right shift (implementation-defined).
template <std::signed_integral T>
constexpr std::make_unsigned_t<T> abs_unsigned_shift(T v) noexcept {
    using U = std::make_unsigned_t<T>;
    const U mask = static_cast<U>(static_cast<T>(v >> (bit_width_v<T> - 1)));
    const U uv = static_cast<U>(v);
    return (uv + mask) ^ mask;
}

// Patented variation (same op count): (v ^ mask) - mask
template <std::signed_integral T>
constexpr std::make_unsigned_t<T> abs_unsigned_xor_sub(T v) noexcept {
    using U = std::make_unsigned_t<T>;
    const U mask = static_cast<U>(-static_cast<U>(v < 0));
    const U uv = static_cast<U>(v);
    return (uv ^ mask) - mask;
}

// Signed result — undefined/overflow if v == numeric_limits<T>::min().
template <std::signed_integral T> constexpr T abs_signed_unsafe(T v) noexcept {
    using U = std::make_unsigned_t<T>;
    const U mask = static_cast<U>(-static_cast<U>(v < 0));
    const U uv = static_cast<U>(v);
    return static_cast<T>((uv + mask) ^ mask);
}

} // namespace bithacks
