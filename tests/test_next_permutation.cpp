#include "test_framework.hpp"

#include "bithacks/next_permutation.hpp"
#include "bithacks/popcount.hpp"

int main() {
    using namespace bithacks;

    EXPECT_EQ(next_bit_permutation(0u), 0u);

    // 0b0011 → 0b0101 → 0b0110 → 0b1001 → 0b1010 → 0b1100
    EXPECT_EQ(next_bit_permutation_div(0b0011u), 0b0101u);
    EXPECT_EQ(next_bit_permutation_ctz(0b0011u), 0b0101u);
    EXPECT_EQ(next_bit_permutation(0b0101u), 0b0110u);
    EXPECT_EQ(next_bit_permutation(0b0110u), 0b1001u);

    // Walk all 5-bit combinations with k set bits; each step keeps popcount.
    // Use a bitmask instead of std::set to avoid allocator exception paths
    // under bugprone-exception-escape.
    for (unsigned k = 1; k <= 5; ++k) {
        unsigned v = (1u << k) - 1u; // start = lowest k bits
        const unsigned limit = 1u << 5;
        unsigned seen_mask = 0;
        unsigned seen_count = 0;
        while (v < limit) {
            EXPECT_EQ(popcount(v), static_cast<int>(k));
            EXPECT(((seen_mask >> v) & 1u) == 0u);
            seen_mask |= (1u << v);
            ++seen_count;
            const unsigned n1 = next_bit_permutation_div(v);
            const unsigned n2 = next_bit_permutation_ctz(v);
            EXPECT_EQ(n1, n2);
            if (n1 <= v || n1 >= limit) {
                break;
            }
            v = n1;
        }
        // C(5,k) combinations
        unsigned comb = 1;
        for (unsigned i = 0; i < k; ++i) {
            comb = comb * (5 - i) / (i + 1);
        }
        EXPECT_EQ(seen_count, comb);
    }

    return test::summary("next_permutation");
}
