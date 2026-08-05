#include "test_framework.hpp"

#include "bithacks/sign.hpp"

int main() {
    using namespace bithacks;

    EXPECT_EQ(sign_neg01(42), 0);
    EXPECT_EQ(sign_neg01(-7), -1);
    EXPECT_EQ(sign_neg01(0), 0);

    EXPECT_EQ(sign_neg01_shift(-1), -1);
    EXPECT_EQ(sign_neg01_unsigned_shift(-1), -1);
    EXPECT_EQ(sign_neg01_unsigned_shift(5), 0);

    EXPECT_EQ(sign_pm1(-3), -1);
    EXPECT_EQ(sign_pm1(3), 1);
    EXPECT_EQ(sign_pm1(0), 1);

    EXPECT_EQ(sign_neg_zero_pos(-9), -1);
    EXPECT_EQ(sign_neg_zero_pos(0), 0);
    EXPECT_EQ(sign_neg_zero_pos(9), 1);
    EXPECT_EQ(sign_neg_zero_pos_shift(-9), -1);
    EXPECT_EQ(sign_neg_zero_pos_shift(0), 0);

    EXPECT_EQ(sign_nonneg01(-1), 0);
    EXPECT_EQ(sign_nonneg01(0), 1);
    EXPECT_EQ(sign_nonneg01(1), 1);

    EXPECT(opposite_signs(-1, 1));
    EXPECT(opposite_signs(1, -1));
    EXPECT(!opposite_signs(1, 2));
    EXPECT(!opposite_signs(-1, -2));
    EXPECT(!opposite_signs(0, 1)); // 0 is non-negative

    return test::summary("sign");
}
