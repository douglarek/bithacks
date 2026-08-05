#include "test_framework.hpp"

#include "bithacks/popcount.hpp"

#include <bit>

int main() {
    using namespace bithacks;

    EXPECT_EQ(popcount_naive(0u), 0);
    EXPECT_EQ(popcount_naive(0b1011u), 3);
    EXPECT_EQ(popcount_kernighan(0b1011u), 3);
    EXPECT_EQ(popcount_table(0xFFFFFFFFu), 32);
    EXPECT_EQ(popcount_parallel(0xFFFFFFFFu), 32);
    EXPECT_EQ(popcount_parallel(0x0u), 0);
    EXPECT_EQ(popcount_parallel(0b10101010u), 4);

    EXPECT_EQ(popcount_mul_14(0b11111111111111u), 14);
    EXPECT_EQ(popcount_mul_24(0xFFFFFFu), 24);
    EXPECT_EQ(popcount_mul_32(0xFFFFFFFFu), 32);

    for (unsigned v = 0; v < 4096; ++v) {
        const int ref = std::popcount(v);
        EXPECT_EQ(popcount_naive(v), ref);
        EXPECT_EQ(popcount_kernighan(v), ref);
        EXPECT_EQ(popcount_table(v), ref);
        EXPECT_EQ(popcount_parallel(v), ref);
    }

    // rank from MSB: top 8 bits of 0xF0000000 have 4 set bits
    EXPECT_EQ(popcount_rank_from_msb(0xF0000000u, 8), 4);
    EXPECT_EQ(popcount_rank_from_msb(0x80000000u, 1), 1);

    // select: first set bit from MSB of 0b0010_1000 is at pos 2 (0-based from
    // MSB in 8-bit view) For full 32-bit 0x00000028 = bits 5 and 3 from LSB →
    // from MSB positions 26 and 28
    EXPECT_EQ(select_bit_from_msb(0x00000028u, 1), 26);
    EXPECT_EQ(select_bit_from_msb(0x00000028u, 2), 28);
    EXPECT_EQ(select_bit_from_msb(0x00000028u, 3), 32);

    return test::summary("popcount");
}
