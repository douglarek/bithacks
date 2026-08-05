#include "test_framework.hpp"

#include "bithacks/swap.hpp"

int main() {
    using namespace bithacks;

    unsigned a = 3, b = 7;
    swap_xor(a, b);
    EXPECT_EQ(a, 7u);
    EXPECT_EQ(b, 3u);
    swap_xor(a, a); // alias safe
    EXPECT_EQ(a, 7u);

    unsigned c = 10, d = 20;
    swap_add(c, d);
    EXPECT_EQ(c, 20u);
    EXPECT_EQ(d, 10u);

    // Swap 4 bits at positions 0 and 4: 0xAB = 1010_1011 → low nibble 1011 with
    // high 1010 0x0F and 0xF0 swap of 0xAB → 0xBA
    EXPECT_EQ(swap_bits(0xABu, BitSwap{0, 4, 4}), 0xBAu);

    return test::summary("swap");
}
