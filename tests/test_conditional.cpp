#include "test_framework.hpp"

#include "bithacks/conditional.hpp"

int main() {
    using namespace bithacks;

    // set bits when flag true
    EXPECT_EQ(set_or_clear_bits(0x00u, 0x0Fu, true), 0x0Fu);
    // clear bits when flag false
    EXPECT_EQ(set_or_clear_bits(0xFFu, 0x0Fu, false), 0xF0u);
    EXPECT_EQ(set_or_clear_bits_xor(0x00u, 0x0Fu, true), 0x0Fu);
    EXPECT_EQ(set_or_clear_bits_xor(0xFFu, 0x0Fu, false), 0xF0u);

    EXPECT_EQ(conditional_negate(5, true), -5);
    EXPECT_EQ(conditional_negate(5, false), 5);
    EXPECT_EQ(conditional_negate(-3, true), 3);

    EXPECT_EQ(conditional_keep_or_negate(5, true), 5);
    EXPECT_EQ(conditional_keep_or_negate(5, false), -5);

    // merge: mask selects bits from b
    EXPECT_EQ(merge_bits(0x00u, 0xFFu, 0x0Fu), 0x0Fu);
    EXPECT_EQ(merge_bits(0xAAu, 0x55u, 0xF0u), 0x5Au);

    return test::summary("conditional");
}
