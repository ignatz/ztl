#include <gtest/gtest.h>

#include "ztl/pythonic.h"

using namespace ZTL;

TEST(Pythonic, Enumerate) {
	typedef std::vector<int> type;
	type vec(10, 42);
	size_t probe = 0;

	for (auto v : enumerate(vec)) {
		ASSERT_EQ(probe++, v.first);
		ASSERT_EQ(42, v.second);
	}

	probe = 0;
	enumerate_proxy<type> ep = enumerate(vec);
	for (auto it = ep.begin(); it != ep.end(); ++it) {
		ASSERT_EQ(probe++, it->first);
		ASSERT_EQ(42, it->second);
	}
}

TEST(Pythonic, Zip) {
	std::vector<int> vec0(10, 42);
	std::vector<int> vec1(10, 5);

	for(auto v : zip(vec0, vec1)) {
		ASSERT_EQ(42, std::get<0>(v));
		ASSERT_EQ( 5, std::get<1>(v));
	}
}

