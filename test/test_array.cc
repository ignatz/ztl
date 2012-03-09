#include <gtest/gtest.h>

#include <boost/archive/text_oarchive.hpp>
#include <sstream>
#include <array>

#include "../include/array.h"

using namespace ZTL;


struct Dummy
{
	constexpr Dummy(int i) : value(i) {}
	int value;
};


TEST(ArrayTest, BasicCheck) {
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
}


TEST(ArrayTest, CopyConstructor) {
	Array<Dummy, 100> a(0);

	for (int ii=0; ii<100; ++ii) {
		a[ii].value = ii;
	}

	Array<Dummy, 100> b(a);
	for (int ii=0; ii<100; ++ii) {
		ASSERT_EQ(a[ii].value, b[ii].value);
	}
}


TEST(ArrayTest, DefaultConstructor) {
	Array<int, 100> a;
	for (int ii=0; ii<100; ++ii) {
		ASSERT_EQ(a[ii], 0);
	}
}


TEST(ArrayTest, Comparisons) {
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


TEST(ArrayTest, Padding) {
	ASSERT_EQ(1+1, sizeof(Array<char, 1>));
	ASSERT_EQ(1, sizeof(Array<char, 0>));
}


TEST(ArrayTest, ZeroSizeArray) {
	Array<char, 0> zero_size;
}


TEST(ArrayTest, Serialization) {
	Array<int, 42> to_serialize;
	std::stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << to_serialize;

	ASSERT_TRUE(ss.str().size());
}


TEST(ArrayTest, Appending) {
	Array<int, 0> a;
	Appending<int, 2> copy_array(a, 2, 2);
	//Appending<int, 3> copy_array2(copy_array, 3);

	ASSERT_EQ(2, copy_array[0]);
	ASSERT_EQ(2, copy_array[1]);

	//for (int ii=0; ii<2; ++ii) {
		//ASSERT_EQ(2, copy_array2[ii]);
		//ASSERT_EQ(2, copy_array[ii]);
	//}
	//ASSERT_EQ(3, copy_array2[2]);

	//Array<int, 5> list(Array<int, 0>(), 0, 1, 2, 3, 4);
	//for (int ii=0; ii<5; ++ii) {
	  //ASSERT_EQ(ii, list[ii]);
	//}
}


TEST(ArrayTest, Conversion) {
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
