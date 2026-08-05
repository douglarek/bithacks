#include "test_framework.hpp"

#include "bithacks/power_of_two.hpp"

#include <bit>

int main() {
    using namespace bithacks;

    EXPECT(is_power_of_two_or_zero(0u));
    EXPECT(is_power_of_two_or_zero(1u));
    EXPECT(is_power_of_two_or_zero(8u));
    EXPECT(!is_power_of_two_or_zero(6u));

    EXPECT(!is_power_of_two(0u));
    EXPECT(is_power_of_two(1u));
    EXPECT(is_power_of_two(1024u));
    EXPECT(!is_power_of_two(1023u));

    for (unsigned v = 0; v < 256; ++v) {
        EXPECT_EQ(is_power_of_two(v), std::has_single_bit(v));
    }

    return test::summary("power_of_two");
}
