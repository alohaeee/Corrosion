#include <gtest/gtest.h>

#include "lexer/rxmap.hpp"
#include "lexer/tokenid.hpp"
using namespace corrosion;
TEST(BasicTests, RxMap)
{
    RxMap<int> rxmap;
    EXPECT_EQ(1, 1);
}
