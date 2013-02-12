#include <gtest/gtest.h>

#include "ztl/numeric.h"
#include "ztl/integral.h"

#include <type_traits>
#include <cmath>

TEST(Numeric, BasicOperations) {
	ASSERT_EQ(  3, (ZTL::add<1,2>::value));
	ASSERT_EQ( -1, (ZTL::sub<1,2>::value));
	ASSERT_EQ( -1, (ZTL::sub<1, 2>::value));
	ASSERT_EQ(120, (ZTL::mult<1,2,3,4,5>::value));
	ASSERT_EQ(  0, (ZTL::div<1,2>::value));
	ASSERT_EQ(  1, (ZTL::mod<1,2>::value));
	ASSERT_EQ(  2, (ZTL::inc<1>::value));
	ASSERT_EQ(  0, (ZTL::dec<1>::value));
	ASSERT_TRUE((ZTL::eq<1,1>::value));
	ASSERT_TRUE((ZTL::eq<-1,-1>::value));
	ASSERT_FALSE((ZTL::eq<-1,1>::value));

	ASSERT_EQ(  0, (ZTL::min<1,2,3,0,3,4>::value));
	ASSERT_EQ(  4, (ZTL::max<1,2,3,0,3,4>::value));
}

template<typename ... Ints>
struct TestUnpackC
{
	enum : int { value = ZTL::add<ZTL::integral_unpack<int, Ints>::value...>::value };
};

TEST(Numeric, Unpack) {
	using ZTL::int_;
	ASSERT_EQ(  15, (TestUnpackC<int_<1>, int_<4>, int_<10>>::value));
}

TEST(Numeric, Pow) {
	ASSERT_EQ(pow(2,0), (ZTL::pow<2, 0>::value));
	ASSERT_EQ(pow(2,1), (ZTL::pow<2, 1>::value));
	ASSERT_EQ(pow(2,8), (ZTL::pow<2, 8>::value));

	ASSERT_EQ(pow(5,5), (ZTL::pow<5, 5>::value));
	ASSERT_EQ(pow(5,5), (ZTL::pow<5, 5>::value));
}

TEST(Numeric, Factorial) {
	using namespace ZTL;
	ASSERT_EQ(size_t(mult<1,2,3,4,5>::value), (factorial<5>::value));
	ASSERT_EQ(size_t(mult<1,2,3,4,5,6,7,8>::value), (factorial<8>::value));
	ASSERT_EQ(1u, (factorial<0>::value));
}

TEST(Numeric, Log2) {
	ASSERT_EQ( 0u, (ZTL::log2<1>::value));
	ASSERT_EQ( 1u, (ZTL::log2<2>::value));
	ASSERT_EQ( 2u, (ZTL::log2<4>::value));
	ASSERT_EQ( 6u, (ZTL::log2<64>::value));
	ASSERT_EQ(10u, (ZTL::log2<1024>::value));
	ASSERT_EQ(10u, (ZTL::log2<1025>::value));
	ASSERT_EQ(10u, (ZTL::log2<2047>::value));
}

TEST(Numeric, Fibonacci) {
	using namespace ZTL;
	ASSERT_EQ( 0u, fibonacci<0>::value);
	ASSERT_EQ( 1u, fibonacci<1>::value);
	ASSERT_EQ( 8u, fibonacci<6>::value);
	ASSERT_EQ(13u, fibonacci<7>::value);
	ASSERT_EQ(21u, fibonacci<8>::value);
}


TEST(IntegralTest, Comparison) {
	using ZTL::int_;
	ASSERT_TRUE(int_<0>() < int_<1>());
	ASSERT_TRUE(int_<1>() > int_<0>());
	ASSERT_TRUE(int_<0>() == int_<0>());
	ASSERT_TRUE(int_<1>() != int_<0>());
	ASSERT_TRUE(int_<0>() <= int_<0>());
	ASSERT_TRUE(int_<0>() >= int_<0>());

	ASSERT_FALSE(int_<0>() > int_<1>());
	ASSERT_FALSE(int_<1>() < int_<0>());
	ASSERT_FALSE(int_<0>() != int_<0>());
	ASSERT_FALSE(int_<1>() == int_<0>());
	ASSERT_FALSE(int_<1>() <= int_<0>());
	ASSERT_FALSE(int_<0>() >= int_<1>());
}
