#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// XOR swap (do not use when both references alias the same object).
template <typename T> constexpr void swap_xor(T &a, T &b) noexcept {
    if (&a == &b) {
        return;
    }
    a ^= b;
    b ^= a;
    a ^= b;
}

// Add/sub swap (overflow risk for signed; prefer unsigned).
template <std::unsigned_integral T>
constexpr void swap_add(T &a, T &b) noexcept {
    if (&a == &b) {
        return;
    }
    a -= b;
    b += a;
    a = b - a;
}

// Describe a bit-field swap: n bits at `i` with n bits at `j` (LSB-based).
// Bundled to avoid adjacent same-type parameters (i/j/n are easy to swap).
struct BitSwap {
    unsigned i; // first field start (from LSB)
    unsigned j; // second field start (from LSB)
    unsigned n; // width in bits
};

// Swap n consecutive bits at positions i and j (0-based from LSB).
// Undefined/incorrect if the ranges overlap.
template <std::unsigned_integral T>
constexpr T swap_bits(T b, BitSwap s) noexcept {
    const T mask = (s.n >= static_cast<unsigned>(bit_width_v<T>))
                       ? ~T{0}
                       : ((T{1} << s.n) - 1);
    const T x = ((b >> s.i) ^ (b >> s.j)) & mask;
    return b ^ ((x << s.i) | (x << s.j));
}

} // namespace bithacks
