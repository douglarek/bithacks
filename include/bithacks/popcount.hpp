#pragma once

#include "bithacks/common.hpp"

#include <array>

namespace bithacks {

// --- Naive: one iteration per bit ---
template <std::unsigned_integral T> constexpr int popcount_naive(T v) noexcept {
    int c = 0;
    for (; v; v >>= 1) {
        c += static_cast<int>(v & 1);
    }
    return c;
}

// --- Brian Kernighan: one iteration per set bit ---
template <std::unsigned_integral T>
constexpr int popcount_kernighan(T v) noexcept {
    int c = 0;
    for (; v; ++c) {
        v &= v - 1; // clear lowest set bit
    }
    return c;
}

// --- Lookup table (byte-wise) ---
namespace detail {
consteval std::array<unsigned char, 256> make_bits_set_table() {
    std::array<unsigned char, 256> t{};
    for (int i = 0; i < 256; ++i) {
        t[static_cast<std::size_t>(i)] = static_cast<unsigned char>(
            (i & 1) + t[static_cast<std::size_t>(i / 2)]);
    }
    return t;
}
inline constexpr auto bits_set_table256 = make_bits_set_table();
} // namespace detail

template <std::unsigned_integral T> constexpr int popcount_table(T v) noexcept {
    int c = 0;
    for (int i = 0; i < bit_width_v<T>; i += 8) {
        c += detail::bits_set_table256[static_cast<std::size_t>((v >> i) &
                                                                0xFFu)];
    }
    return c;
}

// --- Parallel SWAR (32-bit optimized hybrid, ~12 ops) ---
constexpr int popcount_parallel_u32(std::uint32_t v) noexcept {
    v = v - ((v >> 1) & 0x55555555u);
    v = (v & 0x33333333u) + ((v >> 2) & 0x33333333u);
    return static_cast<int>((((v + (v >> 4)) & 0x0F0F0F0Fu) * 0x01010101u) >>
                            24);
}

constexpr int popcount_parallel_u64(std::uint64_t v) noexcept {
    v = v - ((v >> 1) & 0x5555555555555555ULL);
    v = (v & 0x3333333333333333ULL) + ((v >> 2) & 0x3333333333333333ULL);
    v = (v + (v >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return static_cast<int>((v * 0x0101010101010101ULL) >> 56);
}

template <std::unsigned_integral T>
constexpr int popcount_parallel(T v) noexcept {
    if constexpr (bit_width_v<T> <= 32) {
        return popcount_parallel_u32(static_cast<std::uint32_t>(v));
    } else {
        return popcount_parallel_u64(static_cast<std::uint64_t>(v));
    }
}

// --- 64-bit multiply tricks for narrow values ---
// At most 14-bit values:
constexpr int popcount_mul_14(std::uint32_t v) noexcept {
    return static_cast<int>((v * 0x200040008001ULL & 0x111111111111111ULL) %
                            0xF);
}

// 24-bit:
constexpr int popcount_mul_24(std::uint32_t v) noexcept {
    // Two-step form from the classic collection.
    const auto a =
        ((v & 0xFFFu) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1F;
    const auto b =
        (((v & 0xFFF000u) >> 12) * 0x1001001001001ULL & 0x84210842108421ULL) %
        0x1F;
    return static_cast<int>(a + b);
}

// Full 32-bit via three 12-bit groups (approx. 15 ops when 64-bit mul/mod
// cheap):
constexpr int popcount_mul_32(std::uint32_t v) noexcept {
    const auto a =
        ((v & 0xFFFu) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1F;
    const auto b =
        (((v & 0xFFF000u) >> 12) * 0x1001001001001ULL & 0x84210842108421ULL) %
        0x1F;
    const auto c =
        ((v >> 24) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1F;
    return static_cast<int>(a + b + c);
}

// Rank: number of set bits from the MSB down through `pos` bits
// (i.e. popcount of the top `pos` bits). pos in [0, width].
template <std::unsigned_integral T>
constexpr int popcount_rank_from_msb(T v, unsigned pos) noexcept {
    if (pos == 0) {
        return 0;
    }
    if (pos >= static_cast<unsigned>(bit_width_v<T>)) {
        return popcount_parallel(v);
    }
    const T r = v >> (bit_width_v<T> - static_cast<int>(pos));
    return popcount_parallel(r);
}

// Select: find the bit position (from MSB, 0-based) of the r-th set bit
// (r counted from 1). Returns bit_width if rank exceeds popcount.
// Simplified branchy binary-search style for clarity + teaching.
template <std::unsigned_integral T>
constexpr int select_bit_from_msb(T v, unsigned rank) noexcept {
    if (rank == 0) {
        return bit_width_v<T>;
    }
    int seen = 0;
    for (int i = bit_width_v<T> - 1; i >= 0; --i) {
        if ((v >> i) & 1) {
            ++seen;
            if (static_cast<unsigned>(seen) == rank) {
                return bit_width_v<T> - 1 - i; // position from MSB
            }
        }
    }
    return bit_width_v<T>;
}

// Default / recommended hand-rolled implementation.
template <std::unsigned_integral T> constexpr int popcount(T v) noexcept {
    return popcount_parallel(v);
}

// Standard library counterpart: std::popcount(v)

} // namespace bithacks
