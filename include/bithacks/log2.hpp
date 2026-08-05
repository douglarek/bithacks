#pragma once

#include "bithacks/common.hpp"

#include <array>
#include <cstring>

namespace bithacks {

// Floor log2 of a positive integer: position of highest set bit (0-based).
// For v == 0, returns -1 by convention in this tutorial.

// Obvious O(N) scan.
template <std::unsigned_integral T> constexpr int log2_naive(T v) noexcept {
    if (v == 0) {
        return -1;
    }
    int r = 0;
    while (v >>= 1) {
        ++r;
    }
    return r;
}

// Binary-search / O(lg N) bit tests.
template <std::unsigned_integral T> constexpr int log2_binary(T v) noexcept {
    if (v == 0) {
        return -1;
    }
    int r = 0;
    if constexpr (bit_width_v<T> >= 64) {
        if (v & ~T{0xFFFFFFFFu}) {
            v >>= 32;
            r |= 32;
        }
    }
    if constexpr (bit_width_v<T> >= 32) {
        if (v & T{0xFFFF0000u}) {
            v >>= 16;
            r |= 16;
        }
    }
    if (v & T{0xFF00u}) {
        v >>= 8;
        r |= 8;
    }
    if (v & T{0xF0u}) {
        v >>= 4;
        r |= 4;
    }
    if (v & T{0xCu}) {
        v >>= 2;
        r |= 2;
    }
    if (v & T{0x2u}) {
        r |= 1;
    }
    return r;
}

// Lookup table for high byte + shifts.
namespace detail {
consteval std::array<int, 256> make_log2_table() {
    std::array<int, 256> t{};
    t[0] = -1;
    for (int i = 1; i < 256; ++i) {
        int r = 0;
        unsigned x = static_cast<unsigned>(i);
        while (x >>= 1) {
            ++r;
        }
        t[static_cast<std::size_t>(i)] = r;
    }
    return t;
}
inline constexpr auto log2_table256 = make_log2_table();
} // namespace detail

template <std::unsigned_integral T> constexpr int log2_table(T v) noexcept {
    if (v == 0) {
        return -1;
    }
    int r = 0;
    if constexpr (bit_width_v<T> >= 64) {
        if (v >> 32) {
            v >>= 32;
            r += 32;
        }
    }
    if constexpr (bit_width_v<T> >= 32) {
        if (v >> 16) {
            v >>= 16;
            r += 16;
        }
    }
    if (v >> 8) {
        v >>= 8;
        r += 8;
    }
    return r + detail::log2_table256[static_cast<std::size_t>(v)];
}

// De Bruijn multiply + lookup (32-bit).
constexpr int log2_debruijn_u32(std::uint32_t v) noexcept {
    if (v == 0) {
        return -1;
    }
    // Round up to next power of 2 style fill, then multiply.
    // For floor log2 of any v, first propagate bits:
    std::uint32_t x = v;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    // Now x = 2^{floor(log2(v))+1}-1; multiply by De Bruijn sequence.
    static constexpr int table[32] = {
        0, 9,  1,  10, 13, 21, 2,  29, 11, 14, 16, 18, 22, 25, 3, 30,
        8, 12, 20, 28, 15, 17, 24, 7,  19, 27, 23, 6,  26, 5,  4, 31};
    return table[(x * 0x07C4ACDDu) >> 27];
}

// De Bruijn for power-of-two inputs only (classic form).
constexpr int log2_debruijn_pow2_u32(std::uint32_t v) noexcept {
    // v must be power of two.
    static constexpr int table[32] = {
        0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};
    return table[(v * 0x077CB531u) >> 27];
}

// IEEE-754 float exponent trick (32-bit). Not constexpr-friendly on all
// compilers without bit_cast; use std::bit_cast.
inline int log2_float_u32(std::uint32_t v) noexcept {
    if (v == 0) {
        return -1;
    }
    const float f = static_cast<float>(v);
    const auto bits = std::bit_cast<std::uint32_t>(f);
    // Exponent bias 127; mantissa implicit 1 → floor(log2) for normals.
    return static_cast<int>((bits >> 23) & 0xFF) - 127;
}

// Integer log10 via log2 + adjustment table.
constexpr int log10_u32(std::uint32_t v) noexcept {
    if (v == 0) {
        return -1;
    }
    // Powers of 10.
    static constexpr std::uint32_t powers[] = {
        1u,      10u,      100u,      1000u,      10000u,
        100000u, 1000000u, 10000000u, 100000000u, 1000000000u};
    // Approx: log10(v) ≈ log2(v) * log10(2)
    const int lg = log2_binary(v);
    // 1/log2(10) ≈ 1233/4096
    int t = (lg + 1) * 1233 >> 12;
    // Adjust if overestimated.
    while (t > 0 && v < powers[t]) {
        --t;
    }
    while (t < 9 && v >= powers[t + 1]) {
        ++t;
    }
    return t;
}

// Cascaded comparisons (fast for uniform random inputs on average).
constexpr int log10_obvious_u32(std::uint32_t v) noexcept {
    return (v >= 1000000000u)  ? 9
           : (v >= 100000000u) ? 8
           : (v >= 10000000u)  ? 7
           : (v >= 1000000u)   ? 6
           : (v >= 100000u)    ? 5
           : (v >= 10000u)     ? 4
           : (v >= 1000u)      ? 3
           : (v >= 100u)       ? 2
           : (v >= 10u)        ? 1
           : (v >= 1u)         ? 0
                               : -1;
}

// log2 of IEEE-754 binary32 bit pattern (exponent field).
inline int log2_of_float_bits(float f) noexcept {
    const auto bits = std::bit_cast<std::uint32_t>(f);
    const int exp = static_cast<int>((bits >> 23) & 0xFF);
    if (exp == 0) {
        // Subnormal or zero: fall back to mantissa scan.
        const std::uint32_t mant = bits & 0x7FFFFFu;
        if (mant == 0) {
            return std::numeric_limits<int>::min(); // -inf sentinel
        }
        return log2_binary(mant) - 149; // 127 + 23 - 1 bias for subnormals
    }
    if (exp == 255) {
        return std::numeric_limits<int>::max(); // nan/inf sentinel
    }
    return exp - 127;
}

// How many times to halve the binary exponent (root = 2^r).
enum class Pow2RootDepth : unsigned {};

// log2 of pow(2,r)-root of a float: (exponent) / 2^r essentially.
inline int log2_pow2r_root_float(float f, Pow2RootDepth r) noexcept {
    const auto bits = std::bit_cast<std::uint32_t>(f);
    int exp = static_cast<int>((bits >> 23) & 0xFF) - 127;
    return exp >> static_cast<int>(static_cast<unsigned>(r));
}

template <std::unsigned_integral T> constexpr int ilog2(T v) noexcept {
    return log2_binary(v);
}

// std::bit_width(v) - 1  for v > 0; std::countl_zero based.

} // namespace bithacks
