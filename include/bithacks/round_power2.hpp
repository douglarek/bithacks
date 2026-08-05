#pragma once

#include "bithacks/common.hpp"

#include <bit>

namespace bithacks {

// Round up to the next highest power of 2.
// For v == 0, returns 1. For powers of two, returns v itself.
// If v > 2^(width-1), result overflows / wraps — caller must ensure range.

template <std::unsigned_integral T> constexpr T round_up_pow2(T v) noexcept {
    if (v == 0) {
        return 1;
    }
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    if constexpr (bit_width_v<T> >= 16) {
        v |= v >> 8;
    }
    if constexpr (bit_width_v<T> >= 32) {
        v |= v >> 16;
    }
    if constexpr (bit_width_v<T> >= 64) {
        v |= v >> 32;
    }
    return ++v;
}

// Float-based quick/dirty (limited range / precision). Not constexpr.
inline std::uint32_t round_up_pow2_float(std::uint32_t v) noexcept {
    if (v <= 1) {
        return 1;
    }
    const float f = static_cast<float>(v - 1u);
    const auto bits = std::bit_cast<std::uint32_t>(f);
    const int exp = static_cast<int>((bits >> 23) & 0xFF) - 127;
    return 1u << (exp + 1);
}

// C++20: std::bit_ceil(v)

} // namespace bithacks
