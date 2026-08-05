#pragma once

#include "bithacks/common.hpp"

#include <array>

namespace bithacks {

// Count trailing zero bits (position of lowest set bit). For v==0 returns
// width.

template <std::unsigned_integral T>
constexpr int countr_zero_naive(T v) noexcept {
    if (v == 0) {
        return bit_width_v<T>;
    }
    int c = 0;
    while ((v & 1) == 0) {
        v >>= 1;
        ++c;
    }
    return c;
}

// Parallel / SWAR style for 32-bit.
constexpr int countr_zero_parallel_u32(std::uint32_t v) noexcept {
    if (v == 0) {
        return 32;
    }
    // Isolate lowest set bit then count bits below via parallel count of
    // (bit-1).
    std::uint32_t c = 32;
    v &= static_cast<std::uint32_t>(-static_cast<std::int32_t>(v)); // isolate
    if (v) {
        c = 31;
    }
    if (v & 0x0000FFFF) {
        c -= 16;
    }
    if (v & 0x00FF00FF) {
        c -= 8;
    }
    if (v & 0x0F0F0F0F) {
        c -= 4;
    }
    if (v & 0x33333333) {
        c -= 2;
    }
    if (v & 0x55555555) {
        c -= 1;
    }
    return static_cast<int>(c);
}

// Binary search style.
template <std::unsigned_integral T>
constexpr int countr_zero_binary(T v) noexcept {
    if (v == 0) {
        return bit_width_v<T>;
    }
    int c = 0;
    if constexpr (bit_width_v<T> >= 64) {
        if ((v & 0xFFFFFFFFu) == 0) {
            v >>= 32;
            c += 32;
        }
    }
    if constexpr (bit_width_v<T> >= 32) {
        if ((v & 0xFFFFu) == 0) {
            v >>= 16;
            c += 16;
        }
    }
    if ((v & 0xFFu) == 0) {
        v >>= 8;
        c += 8;
    }
    if ((v & 0xFu) == 0) {
        v >>= 4;
        c += 4;
    }
    if ((v & 0x3u) == 0) {
        v >>= 2;
        c += 2;
    }
    if ((v & 0x1u) == 0) {
        c += 1;
    }
    return c;
}

// Float cast method (not constexpr): isolate lowest bit, reinterpret as float,
// read exponent.
inline int countr_zero_float_u32(std::uint32_t v) noexcept {
    if (v == 0) {
        return 32;
    }
    const std::uint32_t isolated =
        v & static_cast<std::uint32_t>(-static_cast<std::int32_t>(v));
    const float f = static_cast<float>(isolated);
    const auto bits = std::bit_cast<std::uint32_t>(f);
    return static_cast<int>((bits >> 23) - 127);
}

// Modulus + lookup (mod 37) for 32-bit.
constexpr int countr_zero_mod37_u32(std::uint32_t v) noexcept {
    if (v == 0) {
        return 32;
    }
    static constexpr int table[37] = {32, 0,  1,  26, 2,  23, 27, 0, 3,  16,
                                      24, 30, 28, 11, 0,  13, 4,  7, 17, 0,
                                      25, 22, 31, 15, 29, 10, 12, 6, 0,  21,
                                      14, 9,  5,  20, 8,  19, 18};
    // Isolate lowest set bit.
    const std::uint32_t iso =
        v & static_cast<std::uint32_t>(-static_cast<std::int32_t>(v));
    return table[iso % 37];
}

// De Bruijn multiply + lookup (32-bit).
constexpr int countr_zero_debruijn_u32(std::uint32_t v) noexcept {
    if (v == 0) {
        return 32;
    }
    static constexpr int table[32] = {
        0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};
    const std::uint32_t iso =
        v & static_cast<std::uint32_t>(-static_cast<std::int32_t>(v));
    return table[(iso * 0x077CB531u) >> 27];
}

// De Bruijn 64-bit.
constexpr int countr_zero_debruijn_u64(std::uint64_t v) noexcept {
    if (v == 0) {
        return 64;
    }
    static constexpr int table[64] = {
        0,  1,  2,  53, 3,  7,  54, 27, 4,  38, 41, 8,  34, 55, 48, 28,
        62, 5,  39, 46, 44, 42, 22, 9,  24, 35, 59, 56, 49, 18, 29, 11,
        63, 52, 6,  26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
        51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12};
    const std::uint64_t iso =
        v & static_cast<std::uint64_t>(-static_cast<std::int64_t>(v));
    return table[(iso * 0x022FDD63CC95386DULL) >> 58];
}

template <std::unsigned_integral T> constexpr int countr_zero(T v) noexcept {
    return countr_zero_binary(v);
}

// std::countr_zero(v)

} // namespace bithacks
