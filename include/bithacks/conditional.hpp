#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// If flag: w |= mask; else w &= ~mask. Branchless.
template <std::unsigned_integral T>
constexpr T set_or_clear_bits(T w, T mask, bool flag) noexcept {
    // Superscalar-friendly form:
    return (w & ~mask) | (bool_mask<T>(flag) & mask);
}

// XOR form from the original page:
template <std::unsigned_integral T>
constexpr T set_or_clear_bits_xor(T w, T mask, bool flag) noexcept {
    return w ^ ((bool_mask<T>(flag) ^ w) & mask);
}

// Conditionally negate: result = flag ? -v : v
template <std::signed_integral T>
constexpr T conditional_negate(T v, bool flag) noexcept {
    // (v ^ -f) + f
    const T f = static_cast<T>(-static_cast<int>(flag));
    return static_cast<T>((v ^ f) + static_cast<T>(flag));
}

// Conditionally keep (don't negate): result = keep ? v : -v
template <std::signed_integral T>
constexpr T conditional_keep_or_negate(T v, bool keep) noexcept {
    // (keep ^ (keep - 1)) * v  →  +1 * v if keep, -1 * v if !keep
    const T k = static_cast<T>(keep);
    return static_cast<T>((k ^ (k - 1)) * v);
}

// Merge bits: for each bit, take from b where mask=1, else from a.
// r = (a & ~mask) | (b & mask)  ≡  a ^ ((a ^ b) & mask)
template <std::unsigned_integral T>
constexpr T merge_bits(T a, T b, T mask) noexcept {
    return a ^ ((a ^ b) & mask);
}

} // namespace bithacks
