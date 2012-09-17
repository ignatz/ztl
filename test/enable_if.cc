#include <gtest/gtest.h>
#include <type_traits>

#include "ztl/enable_if.h"

using namespace ZTL;

template<typename T>
int test(T, typename any_of<T, int, char, float>::type* = 0) { return 42; }

template<typename T>
int test(T, typename non_of<T, int, char, float>::type* = 0) { return 23; }

TEST(EnableTest, BasicCheck) {
	ASSERT_TRUE((std::is_same<typename ZTL::enable_if<true>::type, void>::value));
	ASSERT_TRUE((std::is_same<typename ZTL::disable_if<false>::type, void>::value));

	ASSERT_TRUE((std::is_same<typename ZTL::any_of<int, float, double, int>::type, void>::value));
	ASSERT_TRUE((std::is_same<typename ZTL::non_of<char, float, double, int>::type, void>::value));

	ASSERT_TRUE((std::is_same<typename ZTL::all_of<char, char, char, char>::type, void>::value));

	ASSERT_EQ(42, test((int)0));
	ASSERT_EQ(42, test((float)0));
	ASSERT_EQ(42, test((char)0));

	ASSERT_EQ(23, test((double)0));
	ASSERT_EQ(23, test((unsigned int)0));
}

TEST(EnableTest, IsSame)
{
	ASSERT_TRUE((is_same<int, int, int, int, int, int>::value));
	ASSERT_FALSE((is_same<int, int, int, int, char, int>::value));
}
