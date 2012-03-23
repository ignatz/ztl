#include <gtest/gtest.h>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>

#include "../include/tree.h"

using namespace ZTL;

template<typename T, bool leaf = is_leaf<T>::value>
struct traversal_helper
{
	void operator() (T const& t) {
		for (size_t ii = 0; ii<t.size; ++ii) {
			typename T::child_type const& x = t[ii];
			traversal_helper<typename T::child_type> th;
			th(x);
		}
	}
};

template<typename T>
struct traversal_helper<T, true>
{
	void operator() (T const&) {}
};

typedef SimpleTree<
	ZTL::TreeStructure<
		ZTL::Extend<int, 4>,
		ZTL::Extend<char, 2>,
		ZTL::Extend<char, 3>,
		ZTL::Extend<double, 100>>
	> simpletree_type;
constexpr simpletree_type stree;

TEST(SimpleTree, BasicCheck) {
	ASSERT_EQ(4, stree.size);
	ASSERT_EQ(2, stree[0].size);
	ASSERT_EQ(3, stree[0][0].size);

	ASSERT_EQ(0, stree.level);
	ASSERT_EQ(1, stree[0].level);
	ASSERT_EQ(2, stree[0][0].level);
	ASSERT_EQ(3, stree[0][0][0].level);
}

TEST(SimpleTree, Initialization) {
	simpletree_type st(42, 'A', 'B', 3.141);
}

TEST(SimpleTree, Traits) {
	ASSERT_EQ(true, is_root<simpletree_type>::value);
	ASSERT_EQ(0, is_leaf<simpletree_type>::value);

	ASSERT_EQ(0, is_root<decltype(stree[0][0][0])>::value);
	ASSERT_EQ(true, is_leaf<decltype(stree[0][0][0])>::value);
}

TEST(SimpleTree, Traversal) {
	// down-ward travaersal
	traversal_helper<simpletree_type> th;
	th(stree);
}

TEST(SimpleTree, Serialization) {
	std::stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << stree;

	ASSERT_TRUE(ss.str().size());
}
