#include <gtest/gtest.h>

#include "ztl/functional.h"

using namespace ZTL;

struct A
{
	void fun0() {}
	int  fun1(int) {
		return 1;
	}
};

TEST(FunctionalTest, BasicCheck) {
	A a;
	auto fun0 = bind_mf(&A::fun0, a);
	auto fun1 = bind_mf(&A::fun1, a);

	ASSERT_EQ(1, fun1(42));
}

#include <iostream>
using namespace std;
TEST(FunctionalTest, EnumerateCheck) {
	std::vector<int> vec(10, 42);
	size_t probe = 0;

	for (auto v : enumerate(vec)) {
		ASSERT_EQ(probe++, v.first);
		ASSERT_EQ(42, v.second);
	}

	std::tuple<int, int, int, int> t(0, 1, 2, 3);
	auto u = ZTL::transform(t, typename range<4>::type {} );

	ASSERT_EQ(1, std::get<0>(u));
	//ASSERT_EQ(2, std::get<1>(u));
	//ASSERT_EQ(3, std::get<2>(u));
	//ASSERT_EQ(4, std::get<3>(u));

}
