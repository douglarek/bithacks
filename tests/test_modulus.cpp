#include "test_framework.hpp"

#include "bithacks/modulus.hpp"

int main() {
    using namespace bithacks;

    // mod 2^s
    EXPECT_EQ(mod_power_of_two(100u, 3), 100u % 8);
    EXPECT_EQ(mod_power_of_two(255u, 8), 255u);
    EXPECT_EQ(mod_power_of_two(256u, 8), 0u);

    // mod (2^s - 1)
    for (unsigned s = 1; s <= 8; ++s) {
        const unsigned d = (1u << s) - 1u;
        for (unsigned v = 0; v < 500; ++v) {
            EXPECT_EQ(mod_mersenne_loop(v, s), v % d);
            EXPECT_EQ(mod_mersenne(v, s), v % d);
        }
    }

    // larger
    EXPECT_EQ(mod_mersenne(1000u, 4), 1000u % 15);
    EXPECT_EQ(mod_mersenne(0xFFFFFFFFu, 5), 0xFFFFFFFFu % 31);

    return test::summary("modulus");
}
