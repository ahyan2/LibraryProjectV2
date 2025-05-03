#include <gtest/gtest.h>
#include "../include/my_utils.h"

TEST(MyUtilsTest, AddWorks) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(-1, 1), 0);
}
