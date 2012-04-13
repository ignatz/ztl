#include <gtest/gtest.h>

#include "../include/tuple.h"

using ZTL::call;

int narf(int i, float f0, float f1, double d, char c) {
	return i + f0 + f1 + d + c;
}


TEST(TupleTest, Call) {
	std::function<int(int, float, float, double, char)> fun = &narf;

	int ires = call(fun, 1, 2, std::tuple<float, double>(3, 4), 'A');
	ASSERT_EQ(1+2+3+4+'A', ires);

	std::function<double(int,float,char)> lambda = \
		   [](int i, float f, char c){ return static_cast<double>(f+i+c); };
	double dres = call(lambda, 1, std::tuple<float>(3.14), 'A');
	ASSERT_DOUBLE_EQ(static_cast<float>(3.14)+1+'A', dres);
}
