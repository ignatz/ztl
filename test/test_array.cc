#include <gtest/gtest.h>

#include <boost/archive/text_oarchive.hpp>
#include <sstream>
#include <array>
#include <algorithm>
#include <vector>

#include "../include/array.h"

using namespace ZTL;

// TODO: test for exception guarantees

struct Dummy
{
	constexpr Dummy(int i) : value(i) {}
	int value;
};

struct DummyWithDefault
{
	constexpr DummyWithDefault(int i) : value(i) {}
	constexpr DummyWithDefault() : DummyWithDefault(-1) {}
	int value;
};

struct DummyTwoArgs
{
	int i, j;
	constexpr DummyTwoArgs(int i, int j) : i(i), j(j) {}
};


// check for constexpr (compile check)
Array<int, 10> static_array;
ArrayA<int, 10> static_arraya;
Enum<int, 10> static_enum;

// Testing the StandardArray
TEST(StandardArrayTest, BasicCheck) {
	typedef Array<Dummy, 11> array_type_a;

	// non-const array check
	array_type_a a(42);
	for (int ii=0; ii<10; ++ii) {
		a[ii].value = ii;
		ASSERT_EQ(ii, a[ii].value);
	}
	ASSERT_EQ(42, a[10].value);

	//const array check
	typedef Array<Dummy const, 11> array_type_b;
	constexpr array_type_b b(42);
	for (int ii=0; ii<11; ++ii) {
		ASSERT_EQ(42, b[ii].value);
	}

	Array<char, 10> q;
	Array<char const, 10> r;
	Array<char, 10> const s;
	Array<char const, 10> const t;
}


TEST(StandardArrayTest, Constructors) {
	// default constructor
	Array<int, 100> a;
	for (int ii=0; ii<100; ++ii) {
		ASSERT_EQ(a[ii], 0);
	}

	// copy constructor
	Array<Dummy, 100> b(0);

	for (int ii=0; ii<100; ++ii) {
		b[ii].value = ii;
		ASSERT_EQ(ii, b[ii].value);
	}

	Array<Dummy, 100> c(b);
	for (int ii=0; ii<100; ++ii) {
		ASSERT_EQ(b[ii].value, c[ii].value);
	}

	// constructor on complex types
	// with lvalue ref
	std::vector<int> vec(50, 0);
	int cnt = 0;
	for(auto& v : vec) {
		v = cnt++;
	}
	Array<std::vector<int>, 10> d(vec);
	for (auto& v : d) {
		cnt = 0;
		for (size_t ii=0; ii<50; ++ii) {
			ASSERT_EQ(cnt++, v[ii]);
		}
	}
	// with rvalue ref
	Array<std::vector<int>, 10> e(std::vector<int>(50, 0));
	for (auto& v : e) {
		for (size_t ii=0; ii<50; ++ii) {
			ASSERT_EQ(0, v[ii]);
		}
	}

	int const val = 42;
	Array<int const, 5> f(val);
	for (auto i : f)
		ASSERT_EQ(42, i);
}


TEST(StandardArrayTest, Comparisons) {
	Array<int, 10> m(1);
	Array<int, 10> n(1);
	Array<int, 10> o(2);

	ASSERT_TRUE(m==n);
	ASSERT_TRUE(m<=n);
	ASSERT_TRUE(m>=n);
	ASSERT_TRUE(m!=o);
	ASSERT_TRUE(m<=o);
	ASSERT_TRUE(m<o);
	ASSERT_TRUE(o>m);
}


TEST(StandardArrayTest, Padding) {
	ASSERT_EQ(1+1, sizeof(Array<char, 1>));
	ASSERT_EQ(1, sizeof(Array<char, 0>));
}


TEST(StandardArrayTest, ZeroSizeArray) {
	Array<char, 0> zero_size;
}


TEST(StandardArrayTest, Serialization) {
	Array<int, 42> to_serialize;
	std::stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << to_serialize;

	ASSERT_TRUE(ss.str().size());
}


TEST(StandardArrayTest, InitializerList) {
	int const N = 5;
	Array<int, N>      il1 = {0, 1, 2, 3, 4};
	for (int ii=0; ii<N; ++ii) {
	  ASSERT_EQ(ii, il1[ii]);
	}
}


TEST(StandardArrayTest, Conversion) {
	int const N = 5;
	std::array<int, N> in0 = {{0, 1, 2, 3, 4}};
	Array<int, N>      in1(in0);

	Array<Dummy, N> conv0(in0);
	Array<Dummy, N> conv1(in1);

	for (int ii=0; ii<N; ++ii) {
	  ASSERT_EQ(ii, conv0[ii].value);
	  ASSERT_EQ(ii, conv1[ii].value);
	}
}



// Testing the RecursiveArray
TEST(RecursiveArrayTest, BasicCheck) {

	// non-const array check
	ArrayA<int, 11> a(ArrayA<int, 0>(), 42);
	ASSERT_EQ(42, a[0]);

	ArrayA<int, 1> b = {1};
	ASSERT_EQ(1, b[0]);
	ArrayA<int, 5> c(b, 1,1,1,1);
	for (int ii=0; ii<5; ++ii) {
		ASSERT_EQ(1, c[ii]);
	}

	//const array check
	ArrayA<int const, 10> d = {0,1,2,3,4,5,6,7,8,9};
	for (int ii=0; ii<10; ++ii) {
		ASSERT_EQ(ii, d[ii]);
	}

	ArrayA<char, 10> q;
	ArrayA<char const, 10> r;
	ArrayA<char, 10> const s;
	ArrayA<char const, 10> const t;
}


TEST(RecursiveArrayTest, InitializerList) {
	int const N = 5;
	ArrayA<int, N> il1 = {0, 1, 2, 3, 4};
	for (int ii=0; ii<N; ++ii) {
		ASSERT_EQ(ii, il1[ii]);
	}
}


TEST(RecursiveArrayTest, Constructors) {
	// default
	ArrayA<int, 0> zero;

	ArrayA<DummyWithDefault, 100> dc;
	for (int ii=0; ii<100; ++ii) {
		ASSERT_EQ(-1, dc[ii].value);
	}

	// copy
	ArrayA<double, 10> ccs = {0,1,2,3,4,5,6,7,8,9};
	ArrayA<double, 10> cct(ccs);
	for (int ii=0; ii<10; ++ii) {
		ASSERT_EQ(ii, cct[ii]);
	}
}

TEST(RecursiveArrayTest, Appending) {
	ArrayA<int, 0> a;
	ArrayA<int, 0> a1(a);
	ArrayA<int, 1> b(a, 0);
	ArrayA<int, 2> c0(a, 0, 1);
	ArrayA<int, 2> c1(b, 1);

	ArrayA<int, 6> g(b, 1, 2, 3, 4, 5);

	int cnt = 0;
	for (auto it = g.begin(); it < g.end(); ++it) {
		ASSERT_EQ(*it, cnt);
		++cnt;
	}
}

TEST(RecursiveArrayTest, Serialization) {
	ArrayA<int, 42> to_serialize;
	std::stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << to_serialize;
	ASSERT_TRUE(ss.str().size());
}



// Testing the EnumArray
TEST(EnumArrayTest, BasicCheck) {
	Enum<int, 10> a;
	for (int ii=0; ii<10; ++ii) {
		ASSERT_EQ(ii, a[ii]);
		ASSERT_EQ(ii, a[ii]);
	}

	Enum<DummyTwoArgs, 10> q(42);
	Enum<DummyTwoArgs const, 10> r(42);
	Enum<DummyTwoArgs, 10> const s(42);
	Enum<DummyTwoArgs const, 10> const t(42);
}

TEST(EnumArrayTest, Constructors) {
	Enum<int const, 0> const a;
	Enum<int const, 100> const b;

	// default
	Enum<Dummy const, 100> const dc = {};
	for (int ii=0; ii<100; ++ii) {
		ASSERT_EQ(ii, dc[ii].value);
	}

	// copy
	Enum<Dummy const, 100> cc(dc);
	for (int ii=0; ii<100; ++ii) {
		ASSERT_EQ(ii, cc[ii].value);
	}
}

TEST(EnumArrayTest, InitializerList) {
	int const N = 5;
	Enum<int, N> il1 = {};
	for (int ii=0; ii<N; ++ii) {
		ASSERT_EQ(ii, il1[ii]);
	}
}

TEST(EnumArrayTest, Serialization) {
	Enum<int, 42> to_serialize;
	std::stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << to_serialize;
	ASSERT_TRUE(ss.str().size());
}


TEST(GeneralArrayTest, Misc) {
	ASSERT_TRUE((is_array_impl<typename Array<int, 10>::array_type>::value));
	ASSERT_TRUE((is_array_impl<StandardArray<int, 10, 9>>::value));
	ASSERT_TRUE((is_array_impl<RecursiveArray<int, 10, 10>>::value));
	ASSERT_FALSE((is_array_impl<int>::value));
}

TEST(GeneralArrayTest, Access) {
	// C-style arrays
	int a[10];
	std::for_each(cbegin(a),  cend(a),  [](int) {});
	std::for_each(rbegin(a),  rend(a),  [](int) {});
	std::for_each(crbegin(a), crend(a), [](int) {});

	// std::array<T,N>
	std::array<int, 10> ar;
	std::for_each(cbegin(ar),  cend(ar),  [](int) {});
	std::for_each(rbegin(ar),  rend(ar),  [](int) {});
	std::for_each(crbegin(ar), crend(ar), [](int) {});

	// ZTL::arrays
	Array<int, 10> zar(42);
	std::for_each(cbegin(zar),  cend(zar),  [](int i) { ASSERT_EQ(42, i); });
	std::for_each(rbegin(zar),  rend(zar),  [](int i) { ASSERT_EQ(42, i); });
	std::for_each(crbegin(zar), crend(zar), [](int i) { ASSERT_EQ(42, i); });

	Array<int, 10> const zarc(42);
	std::for_each(cbegin(zarc), cend(zarc), [](int i) { ASSERT_EQ(42, i); });

	Enum<int, 10> ze;
	std::for_each(rbegin(ze), rend(ze), [](int i) {
				  static size_t cnt = 9;
				  ASSERT_EQ(cnt, i);
				  --cnt;
			  });
}
