#pragma once

#include "bithacks/common.hpp"

#include <array>

namespace bithacks {

// Obvious loop reverse of the full word width.
template <std::unsigned_integral T> constexpr T reverse_naive(T v) noexcept {
    T r = 0;
    for (int i = 0; i < bit_width_v<T>; ++i) {
        r = static_cast<T>((r << 1) | (v & 1));
        v >>= 1;
    }
    return r;
}

namespace detail {
consteval std::array<unsigned char, 256> make_bit_reverse_table() {
    std::array<unsigned char, 256> t{};
    for (int i = 0; i < 256; ++i) {
        unsigned char b = static_cast<unsigned char>(i);
        unsigned char r = 0;
        for (int k = 0; k < 8; ++k) {
            r = static_cast<unsigned char>((r << 1) | (b & 1));
            b = static_cast<unsigned char>(b >> 1);
        }
        t[static_cast<std::size_t>(i)] = r;
    }
    return t;
}
inline constexpr auto bit_reverse_table256 = make_bit_reverse_table();
} // namespace detail

template <std::unsigned_integral T> constexpr T reverse_table(T v) noexcept {
    T r = 0;
    for (int i = 0; i < bit_width_v<T>; i += 8) {
        const auto byte = detail::bit_reverse_table256[static_cast<std::size_t>(
            (v >> i) & 0xFFu)];
        r |= static_cast<T>(byte) << (bit_width_v<T> - 8 - i);
    }
    return r;
}

// Reverse a byte with 3 ops (64-bit mul + mod).
constexpr std::uint8_t reverse_byte_mul3(std::uint8_t b) noexcept {
    return static_cast<std::uint8_t>((b * 0x0202020202ULL & 0x010884422010ULL) %
                                     1023);
}

// Reverse a byte with 4 ops (64-bit mul, no div).
constexpr std::uint8_t reverse_byte_mul4(std::uint8_t b) noexcept {
    return static_cast<std::uint8_t>(
        ((b * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32);
}

// Reverse a byte with 7 ops (32-bit only).
constexpr std::uint8_t reverse_byte_mul7(std::uint8_t b) noexcept {
    return static_cast<std::uint8_t>(
        ((b * 0x0802u & 0x22110u) | (b * 0x8020u & 0x88440u)) * 0x10101u >> 16);
}

// Parallel reverse for 32-bit (5 * lg N style).
constexpr std::uint32_t reverse_parallel_u32(std::uint32_t v) noexcept {
    v = ((v >> 1) & 0x55555555u) | ((v & 0x55555555u) << 1);
    v = ((v >> 2) & 0x33333333u) | ((v & 0x33333333u) << 2);
    v = ((v >> 4) & 0x0F0F0F0Fu) | ((v & 0x0F0F0F0Fu) << 4);
    v = ((v >> 8) & 0x00FF00FFu) | ((v & 0x00FF00FFu) << 8);
    v = (v >> 16) | (v << 16);
    return v;
}

constexpr std::uint64_t reverse_parallel_u64(std::uint64_t v) noexcept {
    v = ((v >> 1) & 0x5555555555555555ULL) | ((v & 0x5555555555555555ULL) << 1);
    v = ((v >> 2) & 0x3333333333333333ULL) | ((v & 0x3333333333333333ULL) << 2);
    v = ((v >> 4) & 0x0F0F0F0F0F0F0F0FULL) | ((v & 0x0F0F0F0F0F0F0F0FULL) << 4);
    v = ((v >> 8) & 0x00FF00FF00FF00FFULL) | ((v & 0x00FF00FF00FF00FFULL) << 8);
    v = ((v >> 16) & 0x0000FFFF0000FFFFULL) |
        ((v & 0x0000FFFF0000FFFFULL) << 16);
    v = (v >> 32) | (v << 32);
    return v;
}

template <std::unsigned_integral T> constexpr T reverse_parallel(T v) noexcept {
    if constexpr (bit_width_v<T> <= 32) {
        // Shift into high bits if narrower than 32, reverse, shift back.
        constexpr int w = bit_width_v<T>;
        std::uint32_t x = static_cast<std::uint32_t>(v);
        x = reverse_parallel_u32(x);
        return static_cast<T>(x >> (32 - w));
    } else {
        return static_cast<T>(
            reverse_parallel_u64(static_cast<std::uint64_t>(v)));
    }
}

template <std::unsigned_integral T> constexpr T reverse_bits(T v) noexcept {
    return reverse_parallel(v);
}

// C++23: std::byteswap for byte order; no standard bit-reverse yet.
// GCC/Clang: __builtin_bitreverse32 / __builtin_bitreverse64

} // namespace bithacks
