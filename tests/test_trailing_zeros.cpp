#include "test_framework.hpp"

#include "bithacks/trailing_zeros.hpp"

#include <bit>
#include <cstdint>

int main() {
    using namespace bithacks;

    EXPECT_EQ(countr_zero_naive(0u), 32);
    EXPECT_EQ(countr_zero_naive(1u), 0);
    EXPECT_EQ(countr_zero_naive(2u), 1);
    EXPECT_EQ(countr_zero_naive(8u), 3);
    EXPECT_EQ(countr_zero_naive(0x80000000u), 31);

    for (unsigned v = 0; v < 10000; ++v) {
        const int ref = static_cast<int>(std::countr_zero(v));
        EXPECT_EQ(countr_zero_naive(v), ref);
        EXPECT_EQ(countr_zero_binary(v), ref);
        EXPECT_EQ(countr_zero_parallel_u32(v), ref);
        EXPECT_EQ(countr_zero_mod37_u32(v), ref);
        EXPECT_EQ(countr_zero_debruijn_u32(v), ref);
    }

    // float method
    for (unsigned v = 1; v < 5000; ++v) {
        EXPECT_EQ(countr_zero_float_u32(v),
                  static_cast<int>(std::countr_zero(v)));
    }
    EXPECT_EQ(countr_zero_float_u32(0u), 32);

    // 64-bit De Bruijn
    for (std::uint64_t v = 0; v < 2000; ++v) {
        EXPECT_EQ(countr_zero_debruijn_u64(v),
                  static_cast<int>(std::countr_zero(v)));
    }
    EXPECT_EQ(countr_zero_debruijn_u64(1ull << 40), 40);

    return test::summary("trailing_zeros");
}
