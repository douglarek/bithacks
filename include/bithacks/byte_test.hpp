#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// Parallel byte tests inside a machine word.
// These classic macros operate on all bytes simultaneously.

// True (non-zero mask in high bits of matching bytes) if any byte is zero.
template <std::unsigned_integral T> constexpr T has_zero_byte(T v) noexcept {
    // ((v - 0x010101... ) & ~v & 0x808080...)
    constexpr T ones = static_cast<T>(~T{0} / 255); // 0x010101...
    constexpr T high = static_cast<T>(ones * 0x80); // 0x808080...
    return (v - ones) & ~v & high;
}

constexpr bool has_zero_byte_p(std::uint32_t v) noexcept {
    return has_zero_byte(v) != 0;
}

// Any byte equal to n (0..255).
template <std::unsigned_integral T>
constexpr T has_byte_eq(T x, unsigned n) noexcept {
    constexpr T ones = static_cast<T>(~T{0} / 255);
    return has_zero_byte(static_cast<T>(x ^ (ones * static_cast<T>(n))));
}

constexpr bool has_byte_eq_p(std::uint32_t x, unsigned n) noexcept {
    return has_byte_eq(x, n) != 0;
}

// Any byte < n (n in 1..128 typically for the classic form).
template <std::unsigned_integral T>
constexpr T has_byte_less(T x, unsigned n) noexcept {
    constexpr T ones = static_cast<T>(~T{0} / 255);
    return (x - ones * static_cast<T>(n)) & ~x & (ones * 128);
}

constexpr bool has_byte_less_p(std::uint32_t x, unsigned n) noexcept {
    return has_byte_less(x, n) != 0;
}

// Count bytes < n.
template <std::unsigned_integral T>
constexpr unsigned count_byte_less(T x, unsigned n) noexcept {
    constexpr T ones = static_cast<T>(~T{0} / 255);
    const T y = ((ones * static_cast<T>(127 + n) - (x & (ones * 127))) & ~x &
                 (ones * 128));
    // Sum high bits: each matching byte contributes 0x80.
    unsigned c = 0;
    for (int i = 0; i < bit_width_v<T>; i += 8) {
        if ((y >> i) & 0x80) {
            ++c;
        }
    }
    return c;
}

// Any byte > n.
template <std::unsigned_integral T>
constexpr T has_byte_more(T x, unsigned n) noexcept {
    constexpr T ones = static_cast<T>(~T{0} / 255);
    return ((x + ones * static_cast<T>(127 - n)) | x) & (ones * 128);
}

constexpr bool has_byte_more_p(std::uint32_t x, unsigned n) noexcept {
    return has_byte_more(x, n) != 0;
}

// Count bytes > n.
template <std::unsigned_integral T>
constexpr unsigned count_byte_more(T x, unsigned n) noexcept {
    constexpr T ones = static_cast<T>(~T{0} / 255);
    const T y = ((((x & (ones * 127)) + ones * static_cast<T>(127 - n)) | x) &
                 (ones * 128));
    unsigned c = 0;
    for (int i = 0; i < bit_width_v<T>; i += 8) {
        if ((y >> i) & 0x80) {
            ++c;
        }
    }
    return c;
}

// Any byte in (m, n) — exclusive range, classic form.
// Requires 0 < m <= n < 128 for the common reliable range.
template <std::unsigned_integral T>
constexpr T has_byte_between(T x, unsigned m, unsigned n) noexcept {
    constexpr T ones = static_cast<T>(~T{0} / 255);
    return (ones * static_cast<T>(127 + n) - (x & (ones * 127)) & ~x &
            ((x & (ones * 127)) + ones * static_cast<T>(127 - m))) &
           (ones * 128);
}

constexpr bool has_byte_between_p(std::uint32_t x, unsigned m,
                                  unsigned n) noexcept {
    return has_byte_between(x, m, n) != 0;
}

// Likely-has-between (cheaper, may have rare false positives — see docs).
template <std::unsigned_integral T>
constexpr T likely_has_byte_between(T x, unsigned m, unsigned n) noexcept {
    constexpr T ones = static_cast<T>(~T{0} / 255);
    return (((x - ones * static_cast<T>(n)) & ~x &
             ((x & (ones * 127)) + ones * static_cast<T>(127 - m))) &
            (ones * 128));
}

// Alternative has-zero using +0x7F7F7F7F pattern (original page variant).
constexpr bool has_zero_byte_alt(std::uint32_t v) noexcept {
    return static_cast<bool>(
        ~((((v & 0x7F7F7F7Fu) + 0x7F7F7F7Fu) | v) | 0x7F7F7F7Fu));
}

} // namespace bithacks
