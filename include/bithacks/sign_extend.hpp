#pragma once

#include "bithacks/common.hpp"

namespace bithacks {

// Sign-extend the low `bits` bits of x into a full signed integer.
// Requires: 0 < bits <= bit_width_v<T>
// Upper bits of x beyond `bits` should be zero (or will be masked).
template <std::signed_integral T>
constexpr T sign_extend(T x, unsigned bits) noexcept {
    using U = std::make_unsigned_t<T>;
    if (bits == 0 || bits > static_cast<unsigned>(bit_width_v<T>)) {
        return x;
    }
    if (bits == static_cast<unsigned>(bit_width_v<T>)) {
        return x;
    }
    const U mask = (U{1} << bits) - 1;
    const U m = U{1} << (bits - 1);
    const U xu = static_cast<U>(x) & mask;
    return static_cast<T>((xu ^ m) - m);
}

// Compile-time bit width via template parameter.
// Uses portable arithmetic (no bitfields — bitfield sign is subtle).
template <unsigned Bits, std::signed_integral T>
constexpr T sign_extend_const(T x) noexcept {
    static_assert(Bits > 0 && Bits <= static_cast<unsigned>(bit_width_v<T>));
    return sign_extend(x, Bits);
}

// Variable width in ~3 ops using arithmetic shift (implementation-defined).
// Does not require upper bits of x to already be zero.
template <std::signed_integral T>
constexpr T sign_extend_shift(T x, unsigned bits) noexcept {
    const int s = bit_width_v<T> - static_cast<int>(bits);
    return static_cast<T>((x << s) >> s);
}

// Multiply/divide table form for 32-bit (classic 3-op when tables are free).
// Only for int32_t; bits in [1, 32].
inline constexpr std::int32_t sign_extend_muldiv(std::int32_t x,
                                                 unsigned bits) noexcept {
    // M(B) = 1 << (32 - B)
    static constexpr std::int32_t multipliers[33] = {
        0,
        static_cast<std::int32_t>(1u << 31),
        static_cast<std::int32_t>(1u << 30),
        static_cast<std::int32_t>(1u << 29),
        static_cast<std::int32_t>(1u << 28),
        static_cast<std::int32_t>(1u << 27),
        static_cast<std::int32_t>(1u << 26),
        static_cast<std::int32_t>(1u << 25),
        static_cast<std::int32_t>(1u << 24),
        static_cast<std::int32_t>(1u << 23),
        static_cast<std::int32_t>(1u << 22),
        static_cast<std::int32_t>(1u << 21),
        static_cast<std::int32_t>(1u << 20),
        static_cast<std::int32_t>(1u << 19),
        static_cast<std::int32_t>(1u << 18),
        static_cast<std::int32_t>(1u << 17),
        static_cast<std::int32_t>(1u << 16),
        static_cast<std::int32_t>(1u << 15),
        static_cast<std::int32_t>(1u << 14),
        static_cast<std::int32_t>(1u << 13),
        static_cast<std::int32_t>(1u << 12),
        static_cast<std::int32_t>(1u << 11),
        static_cast<std::int32_t>(1u << 10),
        static_cast<std::int32_t>(1u << 9),
        static_cast<std::int32_t>(1u << 8),
        static_cast<std::int32_t>(1u << 7),
        static_cast<std::int32_t>(1u << 6),
        static_cast<std::int32_t>(1u << 5),
        static_cast<std::int32_t>(1u << 4),
        static_cast<std::int32_t>(1u << 3),
        static_cast<std::int32_t>(1u << 2),
        static_cast<std::int32_t>(1u << 1),
        static_cast<std::int32_t>(1u << 0),
    };
    // divisors[b] = M(b) for b>=2; special-case b==1 uses ~M(1)
    static constexpr std::int32_t divisors[33] = {
        1,
        static_cast<std::int32_t>(~(1u << 31)),
        static_cast<std::int32_t>(1u << 30),
        static_cast<std::int32_t>(1u << 29),
        static_cast<std::int32_t>(1u << 28),
        static_cast<std::int32_t>(1u << 27),
        static_cast<std::int32_t>(1u << 26),
        static_cast<std::int32_t>(1u << 25),
        static_cast<std::int32_t>(1u << 24),
        static_cast<std::int32_t>(1u << 23),
        static_cast<std::int32_t>(1u << 22),
        static_cast<std::int32_t>(1u << 21),
        static_cast<std::int32_t>(1u << 20),
        static_cast<std::int32_t>(1u << 19),
        static_cast<std::int32_t>(1u << 18),
        static_cast<std::int32_t>(1u << 17),
        static_cast<std::int32_t>(1u << 16),
        static_cast<std::int32_t>(1u << 15),
        static_cast<std::int32_t>(1u << 14),
        static_cast<std::int32_t>(1u << 13),
        static_cast<std::int32_t>(1u << 12),
        static_cast<std::int32_t>(1u << 11),
        static_cast<std::int32_t>(1u << 10),
        static_cast<std::int32_t>(1u << 9),
        static_cast<std::int32_t>(1u << 8),
        static_cast<std::int32_t>(1u << 7),
        static_cast<std::int32_t>(1u << 6),
        static_cast<std::int32_t>(1u << 5),
        static_cast<std::int32_t>(1u << 4),
        static_cast<std::int32_t>(1u << 3),
        static_cast<std::int32_t>(1u << 2),
        static_cast<std::int32_t>(1u << 1),
        static_cast<std::int32_t>(1u << 0),
    };
    if (bits == 0 || bits > 32) {
        return x;
    }
    // Must multiply/divide in 32-bit two's complement so that shifting the
    // sign bit into the MSB overflows exactly as on the original page.
    const std::int32_t prod = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(x) *
        static_cast<std::uint32_t>(multipliers[bits]));
    return static_cast<std::int32_t>(prod / divisors[bits]);
}

} // namespace bithacks
