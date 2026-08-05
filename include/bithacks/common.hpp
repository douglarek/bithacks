#pragma once

#include <bit>
#include <climits>
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace bithacks {

// Width helpers
template <std::integral T>
inline constexpr int bit_width_v = static_cast<int>(sizeof(T) * CHAR_BIT);

template <std::integral T> consteval int bit_width() { return bit_width_v<T>; }

// Prefer unsigned when doing pure bit work.
template <std::integral T> using make_unsigned_t = std::make_unsigned_t<T>;

template <std::integral T> using make_signed_t = std::make_signed_t<T>;

// Portable arithmetic right-shift mask for signed values:
// produces all-ones if v < 0, else 0. Avoids implementation-defined
// signed right shift when possible.
template <std::signed_integral T> constexpr T sign_mask(T v) noexcept {
    using U = std::make_unsigned_t<T>;
    return static_cast<T>(static_cast<U>(v) >> (bit_width_v<T> - 1));
}

// Same idea, but returns -1 or 0 as the signed type via comparison.
template <std::signed_integral T> constexpr T sign_mask_portable(T v) noexcept {
    return static_cast<T>(-(v < 0));
}

// All-ones / zero for a boolean flag (true -> all bits 1).
template <std::unsigned_integral U> constexpr U bool_mask(bool f) noexcept {
    return static_cast<U>(-static_cast<std::make_signed_t<U>>(f));
}

template <std::signed_integral S> constexpr S bool_mask(bool f) noexcept {
    return static_cast<S>(-static_cast<int>(f));
}

// Compile-time check that T is a 32-bit or 64-bit integer-like type we target.
template <typename T>
concept word32 = std::unsigned_integral<T> && bit_width_v<T> == 32;

template <typename T>
concept word64 = std::unsigned_integral<T> && bit_width_v<T> == 64;

template <typename T>
concept word32_or_64 = word32<T> || word64<T>;

} // namespace bithacks
