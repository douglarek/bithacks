#pragma once

#include "bithacks/common.hpp"

#include <array>

namespace bithacks {

// Interleave bits of x and y to form Morton / Z-order number.
// Result bit layout: ... y1 x1 y0 x0  (x in even positions)

// Obvious loop for 16-bit inputs → 32-bit result.
constexpr std::uint32_t interleave_naive(std::uint16_t x,
                                         std::uint16_t y) noexcept {
    std::uint32_t z = 0;
    for (int i = 0; i < 16; ++i) {
        z |= (static_cast<std::uint32_t>(x & (1u << i)) << i) |
             (static_cast<std::uint32_t>(y & (1u << i)) << (i + 1));
    }
    return z;
}

// Expand a byte's bits into every other bit (magic numbers).
constexpr std::uint32_t expand_bits_u8(std::uint8_t b) noexcept {
    std::uint32_t x = b;
    x = (x | (x << 8)) & 0x00FF00FFu;
    x = (x | (x << 4)) & 0x0F0F0F0Fu;
    x = (x | (x << 2)) & 0x33333333u;
    x = (x | (x << 1)) & 0x55555555u;
    return x;
}

// Expand 16-bit to 32-bit every-other-bit via binary magic numbers.
constexpr std::uint32_t expand_bits_u16(std::uint16_t b) noexcept {
    std::uint32_t x = b;
    x = (x | (x << 8)) & 0x00FF00FFu;
    x = (x | (x << 4)) & 0x0F0F0F0Fu;
    x = (x | (x << 2)) & 0x33333333u;
    x = (x | (x << 1)) & 0x55555555u;
    return x;
}

constexpr std::uint32_t interleave_magic(std::uint16_t x,
                                         std::uint16_t y) noexcept {
    return expand_bits_u16(x) | (expand_bits_u16(y) << 1);
}

// Table lookup for bytes (partial Morton table: expand each byte).
namespace detail {
consteval std::array<std::uint16_t, 256> make_morton_table() {
    std::array<std::uint16_t, 256> t{};
    for (int i = 0; i < 256; ++i) {
        std::uint16_t x = 0;
        for (int b = 0; b < 8; ++b) {
            if (i & (1 << b)) {
                x |= static_cast<std::uint16_t>(1u << (2 * b));
            }
        }
        t[static_cast<std::size_t>(i)] = x;
    }
    return t;
}
inline constexpr auto morton_table256 = make_morton_table();
} // namespace detail

// Interleave two 16-bit values via byte table.
constexpr std::uint32_t interleave_table(std::uint16_t x,
                                         std::uint16_t y) noexcept {
    return (static_cast<std::uint32_t>(detail::morton_table256[y >> 8]) << 17) |
           (static_cast<std::uint32_t>(detail::morton_table256[x >> 8]) << 16) |
           (static_cast<std::uint32_t>(detail::morton_table256[y & 0xFF])
            << 1) |
           static_cast<std::uint32_t>(detail::morton_table256[x & 0xFF]);
}

// 8-bit × 8-bit coordinates for Morton interleave.
struct ByteCoords {
    std::uint8_t x;
    std::uint8_t y;
};

// 8-bit × 8-bit → 16-bit via 64-bit multiply trick.
constexpr std::uint16_t interleave_mul_u8(ByteCoords c) noexcept {
    const auto xx = ((c.x * 0x0101010101010101ULL & 0x8040201008040201ULL) *
                         0x0102040810204081ULL >>
                     49) &
                    0x5555;
    const auto yy = ((c.y * 0x0101010101010101ULL & 0x8040201008040201ULL) *
                         0x0102040810204081ULL >>
                     48) &
                    0xAAAA;
    return static_cast<std::uint16_t>(xx | yy);
}

constexpr std::uint32_t interleave(std::uint16_t x, std::uint16_t y) noexcept {
    return interleave_magic(x, y);
}

} // namespace bithacks
