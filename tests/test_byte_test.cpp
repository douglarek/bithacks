#include "test_framework.hpp"

#include "bithacks/byte_test.hpp"

#include <cstdint>

namespace {

bool ref_has_zero(std::uint32_t v) {
    for (int i = 0; i < 4; ++i) {
        if (((v >> (8 * i)) & 0xFF) == 0) {
            return true;
        }
    }
    return false;
}

bool ref_has_eq(std::uint32_t v, std::uint8_t n) {
    for (int i = 0; i < 4; ++i) {
        if (((v >> (8 * i)) & 0xFF) == n) {
            return true;
        }
    }
    return false;
}

bool ref_has_less(std::uint32_t v, std::uint8_t n) {
    for (int i = 0; i < 4; ++i) {
        if (((v >> (8 * i)) & 0xFF) < n) {
            return true;
        }
    }
    return false;
}

bool ref_has_more(std::uint32_t v, std::uint8_t n) {
    for (int i = 0; i < 4; ++i) {
        if (((v >> (8 * i)) & 0xFF) > n) {
            return true;
        }
    }
    return false;
}

unsigned ref_count_less(std::uint32_t v, std::uint8_t n) {
    unsigned c = 0;
    for (int i = 0; i < 4; ++i) {
        if (((v >> (8 * i)) & 0xFF) < n) {
            ++c;
        }
    }
    return c;
}

unsigned ref_count_more(std::uint32_t v, std::uint8_t n) {
    unsigned c = 0;
    for (int i = 0; i < 4; ++i) {
        if (((v >> (8 * i)) & 0xFF) > n) {
            ++c;
        }
    }
    return c;
}

struct ExclusiveByteRange {
    std::uint8_t lo;
    std::uint8_t hi;
};

// Bundled range avoids bugprone-easily-swappable-parameters on (m, n).
bool ref_has_between(std::uint32_t word, ExclusiveByteRange range) {
    // exclusive (lo, hi)
    for (int i = 0; i < 4; ++i) {
        const unsigned b = (word >> (8 * i)) & 0xFF;
        if (b > range.lo && b < range.hi) {
            return true;
        }
    }
    return false;
}

} // namespace

int main() {
    using namespace bithacks;

    EXPECT(has_zero_byte_p(0x00112233u));
    EXPECT(!has_zero_byte_p(0x11223344u));
    EXPECT(has_zero_byte_alt(0x00ABCDEF));

    const std::uint32_t samples[] = {
        0x00000000u, 0x01020304u, 0x7F7F7F7Fu, 0x80808080u, 0xFFFFFFFFu,
        0x00FF00FFu, 0x12345678u, 0x0A0B0C0Du, 0x7F000001u, 0x01000000u,
    };

    for (auto v : samples) {
        EXPECT_EQ(has_zero_byte_p(v), ref_has_zero(v));
        for (unsigned n = 0; n < 128; ++n) {
            const auto nb = static_cast<std::uint8_t>(n);
            EXPECT_EQ(has_byte_eq_p(v, n), ref_has_eq(v, nb));
            if (n >= 1) {
                EXPECT_EQ(has_byte_less_p(v, n), ref_has_less(v, nb));
                EXPECT_EQ(count_byte_less(v, n), ref_count_less(v, nb));
            }
            if (n < 127) {
                EXPECT_EQ(has_byte_more_p(v, n), ref_has_more(v, nb));
                EXPECT_EQ(count_byte_more(v, n), ref_count_more(v, nb));
            }
        }
    }

    // between tests for safe range
    for (auto v : samples) {
        for (unsigned m = 1; m < 40; ++m) {
            for (unsigned n = m + 1; n < 80; ++n) {
                EXPECT_EQ(
                    has_byte_between_p(v, m, n),
                    ref_has_between(
                        v, ExclusiveByteRange{static_cast<std::uint8_t>(m),
                                              static_cast<std::uint8_t>(n)}));
            }
        }
    }

    return test::summary("byte_test");
}
