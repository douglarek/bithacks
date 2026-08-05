#pragma once

#include "bithacks/common.hpp"

#include <array>

namespace bithacks {

// Naive: toggle per set bit (Kernighan style).
template <std::unsigned_integral T> constexpr bool parity_naive(T v) noexcept {
    bool p = false;
    while (v) {
        p = !p;
        v &= v - 1;
    }
    return p;
}

namespace detail {
consteval std::array<bool, 256> make_parity_table() {
    std::array<bool, 256> t{};
    for (int i = 0; i < 256; ++i) {
        bool p = false;
        unsigned x = static_cast<unsigned>(i);
        while (x) {
            p = !p;
            x &= x - 1;
        }
        t[static_cast<std::size_t>(i)] = p;
    }
    return t;
}
inline constexpr auto parity_table256 = make_parity_table();
} // namespace detail

template <std::unsigned_integral T> constexpr bool parity_table(T v) noexcept {
    // Fold to a byte then lookup.
    if constexpr (bit_width_v<T> >= 32) {
        v ^= v >> 16;
    }
    if constexpr (bit_width_v<T> >= 16) {
        v ^= v >> 8;
    }
    if constexpr (bit_width_v<T> == 64) {
        v ^= v >> 32;
        v ^= v >> 16;
        v ^= v >> 8;
    }
    return detail::parity_table256[static_cast<std::size_t>(v & 0xFFu)];
}

// Byte via 64-bit multiply and modulus (~4 ops).
constexpr bool parity_mul_byte(std::uint8_t b) noexcept {
    return static_cast<bool>(
        (((b * 0x0101010101010101ULL) & 0x8040201008040201ULL) % 0x1FF) & 1);
}

// Word with multiply (32-bit).
constexpr bool parity_mul_u32(std::uint32_t v) noexcept {
    v ^= v >> 1;
    v ^= v >> 2;
    v = (v & 0x11111111u) * 0x11111111u;
    return static_cast<bool>((v >> 28) & 1);
}

constexpr bool parity_mul_u64(std::uint64_t v) noexcept {
    v ^= v >> 1;
    v ^= v >> 2;
    v = (v & 0x1111111111111111ULL) * 0x1111111111111111ULL;
    return static_cast<bool>((v >> 60) & 1);
}

// Parallel fold + nibble table (0x6996).
template <std::unsigned_integral T>
constexpr bool parity_parallel(T v) noexcept {
    if constexpr (bit_width_v<T> == 64) {
        v ^= v >> 32;
    }
    if constexpr (bit_width_v<T> >= 32) {
        v ^= v >> 16;
    }
    if constexpr (bit_width_v<T> >= 16) {
        v ^= v >> 8;
    }
    v ^= v >> 4;
    v &= 0xF;
    return static_cast<bool>((0x6996 >> v) & 1);
}

template <std::unsigned_integral T> constexpr bool parity(T v) noexcept {
    return parity_parallel(v);
}

// std::popcount(v) & 1

} // namespace bithacks
