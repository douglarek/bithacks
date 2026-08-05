#include "test_framework.hpp"

#include "bithacks/reverse.hpp"

#include <cstdint>

int main() {
    using namespace bithacks;

    EXPECT_EQ(reverse_naive(0u), 0u);
    EXPECT_EQ(reverse_naive(1u), 1u << 31);
    EXPECT_EQ(reverse_naive(0x80000000u), 1u);

    EXPECT_EQ(reverse_parallel_u32(0x00000001u), 0x80000000u);
    EXPECT_EQ(reverse_parallel_u32(0x80000000u), 0x00000001u);
    EXPECT_EQ(reverse_parallel_u32(0x12345678u), reverse_table(0x12345678u));
    EXPECT_EQ(reverse_table(0x12345678u), reverse_naive(0x12345678u));

    for (unsigned b = 0; b < 256; ++b) {
        const auto x = static_cast<std::uint8_t>(b);
        const auto r = reverse_byte_mul3(x);
        EXPECT_EQ(r, reverse_byte_mul4(x));
        EXPECT_EQ(r, reverse_byte_mul7(x));
        EXPECT_EQ(r, static_cast<std::uint8_t>(
                         reverse_naive(static_cast<std::uint32_t>(x)) >> 24));
    }

    EXPECT_EQ(reverse_parallel_u64(1ull), 1ull << 63);

    return test::summary("reverse");
}
