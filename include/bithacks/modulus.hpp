#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// v % (1 << s) without division — obvious mask.
template <std::unsigned_integral T>
constexpr T mod_power_of_two(T v, unsigned s) noexcept {
    if (s >= static_cast<unsigned>(bit_width_v<T>)) {
        return v;
    }
    return v & ((T{1} << s) - 1);
}

// v % ((1 << s) - 1) by repeated digit summing (like digital root in base 2^s).
// Works for s in [1, bit_width). Result in [0, (1<<s)-2] or 0 when divisible
// with the classic loop; we normalize to true remainder.
template <std::unsigned_integral T>
constexpr T mod_mersenne_loop(T v, unsigned s) noexcept {
    if (s == 0) {
        return 0;
    }
    if (s >= static_cast<unsigned>(bit_width_v<T>)) {
        return v;
    }
    const T d = (T{1} << s) - 1;
    if (d == 0) {
        return 0;
    }
    T m = 0;
    for (; v > d; v = m) {
        for (m = 0; v; v >>= s) {
            m += v & d;
        }
    }
    // Now v <= d; final adjust
    return (v == d) ? T{0} : v;
}

// Parallel form for 32-bit modulus by (1<<s)-1 using successive folding.
// s must be a power of two divisor of 32 for the classic mask table approach;
// we implement a general O(lg N) fold that works for any s in 1..31.
constexpr std::uint32_t mod_mersenne_parallel_u32(std::uint32_t v,
                                                  unsigned s) noexcept {
    if (s == 0 || s >= 32) {
        return s == 0 ? 0u : v;
    }
    const std::uint32_t d = (1u << s) - 1u;
    // Fold high parts into low.
    while (v > d) {
        std::uint32_t m = 0;
        std::uint32_t x = v;
        while (x) {
            m += x & d;
            x >>= s;
        }
        v = m;
    }
    return (v == d) ? 0u : v;
}

template <std::unsigned_integral T>
constexpr T mod_mersenne(T v, unsigned s) noexcept {
    if constexpr (bit_width_v<T> <= 32) {
        return static_cast<T>(
            mod_mersenne_parallel_u32(static_cast<std::uint32_t>(v), s));
    } else {
        return mod_mersenne_loop(v, s);
    }
}

} // namespace bithacks
