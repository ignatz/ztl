#include <gtest/gtest.h>

#include "pack.h"

#include <tuple>
#include <type_traits>

using namespace ZTL;

// TODO: check more corner cases

typedef std::tuple<float, double, int, double> my_tuple;

TEST(PackTest, ArgumentPackAccess) {

	ASSERT_TRUE((std::is_same<float,  typename At<0, my_tuple>::type>::value));
	ASSERT_TRUE((std::is_same<double, typename At<1, my_tuple>::type>::value));
	ASSERT_TRUE((std::is_same<int,    typename At<2, my_tuple>::type>::value));
	ASSERT_TRUE((std::is_same<double, typename At<3, my_tuple>::type>::value));

	ASSERT_TRUE((std::is_same<double, typename At<1, int, double, int>::type>::value));

	ASSERT_EQ(4, Arg<0>::get(4,5.3));
	ASSERT_EQ(5.3, Arg<1>::get(4,5.3));
}

TEST(PackTest, ArgumentPackManipulation) {

	// pop_front
	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename PopFront<std::tuple<char, float, double, int, double>>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 std::tuple<>,
					 typename PopFront<std::tuple<char, float, double, int, double>, 5>::type
				 >::value));

	ASSERT_TRUE((std::is_same<my_tuple, typename PopFront<my_tuple, 0>::type>::value));

	// TODO: enable when compiler stops crashing
	// pop_back
	//ASSERT_TRUE((std::is_same<
					 //my_tuple,
					 //typename PopBack<std::tuple<float, double, int, double, char>>::type
				 //>::value));

	//ASSERT_TRUE((std::is_same<
					 //std::tuple<>,
					 //typename PopBack<std::tuple<char, float, double, int, double>, 5>::type
				 //>::value));

	// push_front
	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename PushFront<std::tuple<double, int, double>, float>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename PushFront<std::tuple<float, double, int, double>>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename PushFront<std::tuple<>, float, double, int, double>::type
				 >::value));

	// push_back
	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename PushBack<std::tuple<float, double>, int, double>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename PushBack<std::tuple<float, double, int, double>>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename PushBack<std::tuple<>, float, double, int, double>::type
				 >::value));
}
