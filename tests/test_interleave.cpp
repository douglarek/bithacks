#include "test_framework.hpp"

#include "bithacks/interleave.hpp"

#include <cstdint>

int main() {
    using namespace bithacks;

    EXPECT_EQ(interleave_naive(0, 0), 0u);
    EXPECT_EQ(interleave_naive(1, 0), 1u);
    EXPECT_EQ(interleave_naive(0, 1), 2u);
    EXPECT_EQ(interleave_naive(1, 1), 3u);
    // x=0b11 y=0b01 → bits ... y1 x1 y0 x0 = 0 1 1 1 = 7
    EXPECT_EQ(interleave_naive(0b11, 0b01), 0b0111u);

    for (unsigned x = 0; x < 64; ++x) {
        for (unsigned y = 0; y < 64; ++y) {
            const auto a = interleave_naive(static_cast<std::uint16_t>(x),
                                            static_cast<std::uint16_t>(y));
            const auto b = interleave_magic(static_cast<std::uint16_t>(x),
                                            static_cast<std::uint16_t>(y));
            const auto c = interleave_table(static_cast<std::uint16_t>(x),
                                            static_cast<std::uint16_t>(y));
            EXPECT_EQ(a, b);
            EXPECT_EQ(a, c);
        }
    }

    for (unsigned x = 0; x < 256; ++x) {
        for (unsigned y = 0; y < 256; ++y) {
            const auto ref = static_cast<std::uint16_t>(
                interleave_naive(static_cast<std::uint16_t>(x),
                                 static_cast<std::uint16_t>(y)) &
                0xFFFFu);
            EXPECT_EQ(
                interleave_mul_u8(ByteCoords{static_cast<std::uint8_t>(x),
                                             static_cast<std::uint8_t>(y)}),
                ref);
        }
    }

    return test::summary("interleave");
}
