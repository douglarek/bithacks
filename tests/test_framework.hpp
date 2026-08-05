#pragma once

#include <cstdlib>
#include <iostream>
#include <string_view>

namespace test {

inline int g_failures = 0;
inline int g_checks = 0;

inline void expect(bool cond, std::string_view expr, std::string_view file,
                   int line) {
    ++g_checks;
    if (!cond) {
        ++g_failures;
        std::cerr << "FAIL " << file << ":" << line << "  " << expr << '\n';
    }
}

inline int summary(std::string_view name) {
    if (g_failures == 0) {
        std::cout << "[PASS] " << name << " (" << g_checks << " checks)\n";
        return 0;
    }
    std::cerr << "[FAIL] " << name << " (" << g_failures << "/" << g_checks
              << " failed)\n";
    return 1;
}

} // namespace test

#define EXPECT(cond)                                                           \
    ::test::expect(static_cast<bool>(cond), #cond, __FILE__, __LINE__)

#define EXPECT_EQ(a, b)                                                        \
    ::test::expect((a) == (b), #a " == " #b, __FILE__, __LINE__)
