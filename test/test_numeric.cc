#include <gtest/gtest.h>

#include "numeric.h"

#include <type_traits>

using namespace ZTL;

TEST(NumericTest, BasicOperations) {
	ASSERT_EQ(  3, (add_c<1,2>::type::value));
	ASSERT_EQ( -1, (sub<int_<1>,int_<2>>::type::value));
	ASSERT_EQ( -1, (sub_c<1, 2>::type::value));
	ASSERT_EQ(120, (mult_c<1,2,3,4,5>::type::value));
	ASSERT_EQ(  0, (div_c<1,2>::type::value));
	ASSERT_EQ(  1, (mod_c<1,2>::type::value));
	ASSERT_EQ(  2, (inc_c<1>::type::value));
	ASSERT_EQ(  0, (dec<int_<1>>::type::value));

	ASSERT_EQ(  0, (min_c<1,2,3,0,3,4>::type::value));
	ASSERT_EQ(  4, (max_c<1,2,3,0,3,4>::type::value));
}

TEST(NumericTest, Factorial) {
	ASSERT_EQ((mult_c<1,2,3,4,5>::type::value), (factorial_c<5>::type::value));
	ASSERT_EQ((mult_c<1,2,3,4,5,6,7,8>::type::value), (factorial_c<8>::type::value));
	ASSERT_EQ(1, (factorial<int_<0>>::type::value));
}

TEST(NumericTest, Fibonacci) {
	ASSERT_EQ(0 , fibonacci_c<0>::type::value);
	ASSERT_EQ(1 , fibonacci_c<1>::type::value);
	ASSERT_EQ(8 , fibonacci<int_<6>>::type::value);
	ASSERT_EQ(13, fibonacci<int_<7>>::type::value);
	ASSERT_EQ(21, fibonacci<int_<8>>::type::value);
}
