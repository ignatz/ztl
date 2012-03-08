#include <gtest/gtest.h>

#include "../include/pack.h"

#include <tuple>
#include <type_traits>

using namespace ZTL;

// TODO: check more corner cases

typedef std::tuple<float, double, int, double> my_tuple;

TEST(PackTest, ArgumentPackAccess) {

	ASSERT_TRUE((std::is_same<float,  typename get<0, my_tuple>::type>::value));
	ASSERT_TRUE((std::is_same<double, typename get<1, my_tuple>::type>::value));
	ASSERT_TRUE((std::is_same<int,    typename get<2, my_tuple>::type>::value));
	ASSERT_TRUE((std::is_same<double, typename get<3, my_tuple>::type>::value));

	ASSERT_TRUE((std::is_same<double, typename get<1, int, double, int>::type>::value));

	ASSERT_EQ(4, arg<0>::get(4,5.3));
	ASSERT_EQ(5.3, arg<1>::get(4,5.3));

	ASSERT_EQ(4, (size<my_tuple>::value));
}

TEST(PackTest, Copying) {

	ASSERT_TRUE((std::is_same<typename copy<my_tuple, std::tuple>::type, my_tuple>::value));

	ASSERT_TRUE((std::is_same<typename copy_n<std::tuple<float, double, int, double, char>, std::tuple, 4>::type, my_tuple>::value));
}

TEST(PackTest, ArgumentPackManipulation) {

	// pop_front
	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename pop_front<std::tuple<char, float, double, int, double>>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 std::tuple<>,
					 typename pop_front<std::tuple<char, float, double, int, double>, 5>::type
				 >::value));

	ASSERT_TRUE((std::is_same<my_tuple, typename pop_front<my_tuple, 0>::type>::value));

	// pop_back
	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename pop_back<std::tuple<float, double, int, double, char>>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 std::tuple<>,
					 typename pop_back<std::tuple<char, float, double, int, double>, 5>::type
				 >::value));

	// push_front
	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename push_front<std::tuple<double, int, double>, float>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename push_front<std::tuple<float, double, int, double>>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename push_front<std::tuple<>, float, double, int, double>::type
				 >::value));

	// push_back
	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename push_back<std::tuple<float, double>, int, double>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename push_back<std::tuple<float, double, int, double>>::type
				 >::value));

	ASSERT_TRUE((std::is_same<
					 my_tuple,
					 typename push_back<std::tuple<>, float, double, int, double>::type
				 >::value));
}

TEST(PackTest, ForEach) {

	int const N = 5;
	int ar[N];
	std::function<int (float, int*&)> f = [](float x, int*& pos) { *pos = x+2; ++pos; return x+2; };
	std::function<int (float)> f0 = std::bind(f, std::placeholders::_1, ar);

	for_each(f0, 0, 1, 2, 3, 4);
	for (int ii = 0; ii< N; ++ii) {
		ASSERT_EQ(f0(ii), ar[ii]);
	}
}

