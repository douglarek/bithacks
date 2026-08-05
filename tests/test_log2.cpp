#include "test_framework.hpp"

#include "bithacks/log2.hpp"

#include <bit>
#include <cmath>

int main() {
    using namespace bithacks;

    EXPECT_EQ(log2_naive(0u), -1);
    EXPECT_EQ(log2_naive(1u), 0);
    EXPECT_EQ(log2_naive(2u), 1);
    EXPECT_EQ(log2_naive(3u), 1);
    EXPECT_EQ(log2_naive(1024u), 10);

    for (unsigned v = 1; v < 10000; ++v) {
        const int ref = static_cast<int>(std::bit_width(v) - 1);
        EXPECT_EQ(log2_naive(v), ref);
        EXPECT_EQ(log2_binary(v), ref);
        EXPECT_EQ(log2_table(v), ref);
        EXPECT_EQ(log2_debruijn_u32(v), ref);
    }

    // power-of-two De Bruijn
    for (int i = 0; i < 32; ++i) {
        EXPECT_EQ(log2_debruijn_pow2_u32(1u << i), i);
    }

    // float method — skip values that don't fit exactly in float mantissa
    for (unsigned v = 1; v < 1u << 24; v = v * 3 / 2 + 1) {
        EXPECT_EQ(log2_float_u32(v), static_cast<int>(std::bit_width(v) - 1));
    }

    EXPECT_EQ(log10_u32(0), -1);
    EXPECT_EQ(log10_u32(1), 0);
    EXPECT_EQ(log10_u32(9), 0);
    EXPECT_EQ(log10_u32(10), 1);
    EXPECT_EQ(log10_u32(99), 1);
    EXPECT_EQ(log10_u32(100), 2);
    EXPECT_EQ(log10_u32(1000000000u), 9);

    for (unsigned v = 1; v < 100000; ++v) {
        EXPECT_EQ(log10_u32(v), log10_obvious_u32(v));
        EXPECT_EQ(
            log10_obvious_u32(v),
            static_cast<int>(std::floor(std::log10(static_cast<double>(v)))));
    }

    // float bit pattern log2
    EXPECT_EQ(log2_of_float_bits(1.0f), 0);
    EXPECT_EQ(log2_of_float_bits(2.0f), 1);
    EXPECT_EQ(log2_of_float_bits(8.0f), 3);
    EXPECT_EQ(log2_of_float_bits(0.5f), -1);

    EXPECT_EQ(log2_pow2r_root_float(16.0f, Pow2RootDepth{1}),
              2); // log2(sqrt(16)) = 2
    EXPECT_EQ(log2_pow2r_root_float(16.0f, Pow2RootDepth{2}),
              1); // log2(16^(1/4)) = 1

    return test::summary("log2");
}
