#include "bithacks/abs.hpp"
#include "bithacks/byte_test.hpp"
#include "bithacks/conditional.hpp"
#include "bithacks/interleave.hpp"
#include "bithacks/log2.hpp"
#include "bithacks/minmax.hpp"
#include "bithacks/modulus.hpp"
#include "bithacks/next_permutation.hpp"
#include "bithacks/parity.hpp"
#include "bithacks/popcount.hpp"
#include "bithacks/power_of_two.hpp"
#include "bithacks/reverse.hpp"
#include "bithacks/round_power2.hpp"
#include "bithacks/sign.hpp"
#include "bithacks/sign_extend.hpp"
#include "bithacks/swap.hpp"
#include "bithacks/trailing_zeros.hpp"

#include <cstdio>

namespace {

void section(const char *title) { std::printf("\n=== %s ===\n", title); }

// Print low Width bits without std::bitset (avoids exception-escape noise).
template <int Width> void print_bits(unsigned v) {
    static_assert(Width > 0 && Width <= 32);
    for (int i = Width - 1; i >= 0; --i) {
        std::putchar(((v >> i) & 1u) ? '1' : '0');
    }
}

} // namespace

int main() {
    using namespace bithacks;

    std::puts("Bit Twiddling Hacks — C++23 demo");
    std::puts("Source techniques: "
              "https://graphics.stanford.edu/~seander/bithacks.html");

    section("Sign / abs / minmax");
    for (int v : {-5, 0, 5}) {
        std::printf("  v=%d sign=%d abs=%u\n", v, sign_neg_zero_pos(v),
                    abs_unsigned(v));
    }
    std::printf("  min(-3, 7)=%d max(-3, 7)=%d\n", min_branchless(-3, 7),
                max_branchless(-3, 7));
    std::printf("  opposite_signs(-1, 2)=%s\n",
                opposite_signs(-1, 2) ? "true" : "false");

    section("Power of two / round up");
    for (unsigned v : {0u, 1u, 3u, 8u, 100u}) {
        std::printf("  v=%u is_pow2=%s ceil_pow2=%u\n", v,
                    is_power_of_two(v) ? "true" : "false", round_up_pow2(v));
    }

    section("Sign extend (4-bit)");
    for (int x : {0b0001, 0b0111, 0b1000, 0b1111}) {
        std::printf("  0b");
        print_bits<4>(static_cast<unsigned>(x));
        std::printf(" -> %d\n", sign_extend(x, 4));
    }

    section("Conditional ops / merge");
    {
        unsigned w = 0xA5;
        std::printf("  set low nibble: %x\n",
                    set_or_clear_bits(w, 0x0Fu, true));
        std::printf("  cond_neg(5,true)=%d\n", conditional_negate(5, true));
        std::printf("  merge(0xAA,0x55,0xF0)=%x\n",
                    merge_bits(0xAAu, 0x55u, 0xF0u));
    }

    section("Popcount / parity");
    for (unsigned v : {0u, 0b1011u, 0xFFu, 0xFFFFFFFFu}) {
        std::printf("  v=0x%x pop=%d parity=%s\n", v, popcount(v),
                    parity(v) ? "true" : "false");
    }

    section("Reverse / swap bits");
    std::printf("  reverse(0x12345678)=0x%x\n", reverse_bits(0x12345678u));
    std::printf("  swap_bits(0xAB,0,4,4)=0x%x\n",
                swap_bits(0xABu, BitSwap{0, 4, 4}));

    section("Modulus / log2 / ctz");
    std::printf("  100 %% 8 (mask)=%u\n", mod_power_of_two(100u, 3));
    std::printf("  1000 %% 15 (mersenne)=%u\n", mod_mersenne(1000u, 4));
    std::printf("  ilog2(1000)=%d log10(1000)=%d\n", ilog2(1000u),
                log10_u32(1000u));
    std::printf("  ctz(0x28)=%d\n", countr_zero(0x28u));

    section("Interleave (Morton) / next perm");
    std::printf("  interleave(3,1)=%u\n", interleave(3, 1));
    unsigned p = 0b0011u;
    std::printf("  next perms of 0b0011:");
    for (int i = 0; i < 6; ++i) {
        std::printf(" 0b");
        print_bits<4>(p);
        p = next_bit_permutation(p);
    }
    std::putchar('\n');

    section("Byte tests");
    const unsigned word = 0x00112233u;
    std::printf("  has_zero(0x00112233)=%s\n",
                has_zero_byte_p(word) ? "true" : "false");
    std::printf("  has_eq(0x00112233, 0x11)=%s\n",
                has_byte_eq_p(word, 0x11) ? "true" : "false");
    std::printf("  has_less(0x11223344, 0x20)=%s\n",
                has_byte_less_p(0x11223344u, 0x20) ? "true" : "false");

    std::puts("\nDone. See docs/ for full Chinese tutorials.");
    return 0;
}
