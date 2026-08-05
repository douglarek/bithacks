#include "test_framework.hpp"

#include "bithacks/abs.hpp"

#include <limits>

int main() {
    using namespace bithacks;

    EXPECT_EQ(abs_unsigned(0), 0u);
    EXPECT_EQ(abs_unsigned(42), 42u);
    EXPECT_EQ(abs_unsigned(-42), 42u);
    EXPECT_EQ(abs_unsigned_shift(-7), 7u);
    EXPECT_EQ(abs_unsigned_xor_sub(-100), 100u);

    // INT_MIN: unsigned abs is well-defined as 2^31
    constexpr auto imin = std::numeric_limits<int>::min();
    EXPECT_EQ(abs_unsigned(imin),
              static_cast<unsigned>(static_cast<unsigned>(imin)));

    EXPECT_EQ(abs_signed_unsafe(-5), 5);
    EXPECT_EQ(abs_signed_unsafe(5), 5);

    return test::summary("abs");
}
