#include "test_framework.hpp"

#include "bithacks/minmax.hpp"

int main() {
    using namespace bithacks;

    EXPECT_EQ(min_branchless(3, 5), 3);
    EXPECT_EQ(min_branchless(5, 3), 3);
    EXPECT_EQ(max_branchless(3, 5), 5);
    EXPECT_EQ(max_branchless(5, 3), 5);
    EXPECT_EQ(min_branchless(-3, 5), -3);
    EXPECT_EQ(max_branchless(-3, -8), -3);

    EXPECT_EQ(min_quick(10, 20), 10);
    EXPECT_EQ(max_quick(10, 20), 20);

    EXPECT_EQ(min_branchless(3u, 5u), 3u);
    EXPECT_EQ(max_branchless(3u, 5u), 5u);

    return test::summary("minmax");
}
