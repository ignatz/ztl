#include <gtest/gtest.h>

#include "ztl/functional.h"
#include "ztl/runtime.h"

using namespace ZTL;

struct A
{
	void fun0() {}
	int  fun1(int) {
		return 1;
	}
};

TEST(Functional, BasicCheck) {
	A a;
	auto fun0 = bind_mf(&A::fun0, a);
	auto fun1 = bind_mf(&A::fun1, a);

	ASSERT_EQ(1, fun1(42));
}

TEST(Functional, Transform) {
	std::function<float(int)> f = [](int i) { return (float)i + 0.1; };

	std::tuple<int, int, int, int> t(0, 1, 2, 3);
	std::tuple<std::function<float(int)>,
		std::function<float(int)>,
		std::function<float(int)>,
		std::function<float(int)>> u(f, f, f, f);

	//ASSERT_FLOAT_EQ(1.1, transform_helper<1>(t, u));
	auto v = transform(t, u);
	auto w = transform(t, f);

	ASSERT_FLOAT_EQ(0.1, std::get<0>(v));
	ASSERT_FLOAT_EQ(1.1, std::get<1>(v));
	ASSERT_FLOAT_EQ(2.1, std::get<2>(v));
	ASSERT_FLOAT_EQ(3.1, std::get<3>(v));

	ASSERT_FLOAT_EQ(0.1, std::get<0>(w));
	ASSERT_FLOAT_EQ(1.1, std::get<1>(w));
	ASSERT_FLOAT_EQ(2.1, std::get<2>(w));
	ASSERT_FLOAT_EQ(3.1, std::get<3>(w));
}
