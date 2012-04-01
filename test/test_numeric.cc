#include <gtest/gtest.h>

#include "../include/numeric.h"

#include <type_traits>
#include <cmath>

TEST(NumericTest, BasicOperations) {
	ASSERT_EQ(  3, (ZTL::add<1,2>::value));
	ASSERT_EQ( -1, (ZTL::sub<1,2>::value));
	ASSERT_EQ( -1, (ZTL::sub<1, 2>::value));
	ASSERT_EQ(120, (ZTL::mult<1,2,3,4,5>::value));
	ASSERT_EQ(  0, (ZTL::div<1,2>::value));
	ASSERT_EQ(  1, (ZTL::mod<1,2>::value));
	ASSERT_EQ(  2, (ZTL::inc<1>::value));
	ASSERT_EQ(  0, (ZTL::dec<1>::value));

	ASSERT_EQ(  0, (ZTL::min<1,2,3,0,3,4>::value));
	ASSERT_EQ(  4, (ZTL::max<1,2,3,0,3,4>::value));
}

template<typename ... Ints>
struct TestUnpackC
{
	enum : int { value = ZTL::add<ZTL::unpack_c<Ints>::value...>::value };
};

TEST(NumericTest, Unpack) {
	using ZTL::int_;
	ASSERT_EQ(  15, (TestUnpackC<int_<1>, int_<4>, int_<10>>::value));
}

TEST(NumericTest, Pow) {
	ASSERT_EQ(pow(2,0), (ZTL::pow<2, 0>::value));
	ASSERT_EQ(pow(2,1), (ZTL::pow<2, 1>::value));
	ASSERT_EQ(pow(2,8), (ZTL::pow<2, 8>::value));

	ASSERT_EQ(pow(5,5), (ZTL::pow<5, 5>::value));
	ASSERT_EQ(pow(5,5), (ZTL::pow<5, 5>::value));
}

TEST(NumericTest, Factorial) {
	using namespace ZTL;
	ASSERT_EQ((mult<1,2,3,4,5>::value), (factorial<5>::value));
	ASSERT_EQ((mult<1,2,3,4,5,6,7,8>::value), (factorial<8>::value));
	ASSERT_EQ(1, (factorial<0>::value));
}

TEST(NumericTest, Fibonacci) {
	using namespace ZTL;
	ASSERT_EQ(0 , fibonacci<0>::value);
	ASSERT_EQ(1 , fibonacci<1>::value);
	ASSERT_EQ(8 , fibonacci<6>::value);
	ASSERT_EQ(13, fibonacci<7>::value);
	ASSERT_EQ(21, fibonacci<8>::value);
}
