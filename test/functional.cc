#include <gtest/gtest.h>

#include "../include/functional.h"

using ZTL::bind_mf;

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
