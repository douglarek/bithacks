#include "test_framework.hpp"

#include "bithacks/sign_extend.hpp"

int main() {
    using namespace bithacks;

    // 4-bit two's complement: 0b0111 = 7, 0b1000 = -8, 0b1111 = -1
    EXPECT_EQ(sign_extend(0b0111, 4), 7);
    EXPECT_EQ(sign_extend(0b1000, 4), -8);
    EXPECT_EQ(sign_extend(0b1111, 4), -1);
    EXPECT_EQ(sign_extend(0b0001, 4), 1);

    EXPECT_EQ((sign_extend_const<5>(0b11111)), -1);
    EXPECT_EQ((sign_extend_const<5>(0b01111)), 15);
    EXPECT_EQ((sign_extend_const<5>(0b10000)), -16);

    EXPECT_EQ(sign_extend_shift(0b1000, 4), -8);
    EXPECT_EQ(sign_extend_shift(0b0111, 4), 7);

    EXPECT_EQ(sign_extend_muldiv(0b1000, 4), -8);
    EXPECT_EQ(sign_extend_muldiv(0b0111, 4), 7);
    EXPECT_EQ(sign_extend_muldiv(1, 1), -1); // 1-bit: only -1

    return test::summary("sign_extend");
}
