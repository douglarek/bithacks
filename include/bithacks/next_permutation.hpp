#pragma once

#include "bithacks/common.hpp"
#include "bithacks/trailing_zeros.hpp"

namespace bithacks {

// Lexicographically next bit permutation with the same number of set bits.
// Example: 00101 → 00110 → 01001 → ...
// For v == 0, returns 0.

// Portable form using division (no compiler ctz builtin required).
template <std::unsigned_integral T>
constexpr T next_bit_permutation_div(T v) noexcept {
    if (v == 0) {
        return 0;
    }
    const T t = (v | (v - 1)) + 1;
    // ((t & -t) / (v & -v) >> 1) - 1
    const T lowest_t =
        t & static_cast<T>(-static_cast<std::make_signed_t<T>>(t));
    const T lowest_v =
        v & static_cast<T>(-static_cast<std::make_signed_t<T>>(v));
    return t | (((lowest_t / lowest_v) >> 1) - 1);
}

// Form using trailing-zero count (matches the __builtin_ctz version).
template <std::unsigned_integral T>
constexpr T next_bit_permutation_ctz(T v) noexcept {
    if (v == 0) {
        return 0;
    }
    const T t = v | (v - 1);
    const int c = countr_zero(v);
    // (~t & -~t) isolates the lowest zero of t (i.e. carry-out bit).
    const T nt = static_cast<T>(~t);
    const T lowest_zero =
        nt & static_cast<T>(-static_cast<std::make_signed_t<T>>(nt));
    return static_cast<T>((t + 1) | (((lowest_zero - 1) >> (c + 1))));
}

template <std::unsigned_integral T>
constexpr T next_bit_permutation(T v) noexcept {
    return next_bit_permutation_div(v);
}

} // namespace bithacks
