#include "test_framework.hpp"

#include "bithacks/round_power2.hpp"

#include <bit>

int main() {
    using namespace bithacks;

    EXPECT_EQ(round_up_pow2(0u), 1u);
    EXPECT_EQ(round_up_pow2(1u), 1u);
    EXPECT_EQ(round_up_pow2(2u), 2u);
    EXPECT_EQ(round_up_pow2(3u), 4u);
    EXPECT_EQ(round_up_pow2(5u), 8u);
    EXPECT_EQ(round_up_pow2(100u), 128u);

    for (unsigned v = 1; v < 10000; ++v) {
        EXPECT_EQ(round_up_pow2(v), std::bit_ceil(v));
    }

    // float path for modest range
    for (unsigned v = 1; v < 1u << 22; v = v * 2 + 3) {
        EXPECT_EQ(round_up_pow2_float(v), std::bit_ceil(v));
    }

    return test::summary("round_power2");
}
