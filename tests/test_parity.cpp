#include "test_framework.hpp"

#include "bithacks/parity.hpp"

#include <bit>
#include <cstdint>

int main() {
    using namespace bithacks;

    EXPECT_EQ(parity_naive(0u), false);
    EXPECT_EQ(parity_naive(1u), true);
    EXPECT_EQ(parity_naive(3u), false);
    EXPECT_EQ(parity_naive(7u), true);

    for (unsigned v = 0; v < 2048; ++v) {
        const bool ref = (std::popcount(v) & 1) != 0;
        EXPECT_EQ(parity_naive(v), ref);
        EXPECT_EQ(parity_table(v), ref);
        EXPECT_EQ(parity_parallel(v), ref);
        EXPECT_EQ(parity_mul_u32(v), ref);
    }

    for (unsigned b = 0; b < 256; ++b) {
        const bool ref = (std::popcount(b) & 1) != 0;
        EXPECT_EQ(parity_mul_byte(static_cast<std::uint8_t>(b)), ref);
    }

    return test::summary("parity");
}
