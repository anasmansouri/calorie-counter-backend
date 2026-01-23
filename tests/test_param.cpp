#include <gtest/gtest.h>
#include <vector>
#include <tuple>
#include "math.hpp"

class AddParam : public ::testing::TestWithParam<std::tuple<int,int,int>> {};
TEST_P(AddParam, Works) {
  auto [a,b,sum] = GetParam();
  EXPECT_EQ(add(a,b), sum);
}
INSTANTIATE_TEST_SUITE_P(
  Table, AddParam,
  ::testing::Values(
    std::make_tuple(2,3,5),
    std::make_tuple(-1,1,0),
    std::make_tuple(0,0,0)
  )
);

// Typed tests example
template<class T>
class NumericOps : public ::testing::Test {};
using NumericTypes = ::testing::Types<int, long long>;
TYPED_TEST_SUITE(NumericOps, NumericTypes);

TYPED_TEST(NumericOps, AddIsClosed) {
  TypeParam a = 4, b = 5;
  EXPECT_EQ(add((int)a,(int)b), 9);
}

