#include <gtest/gtest.h>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>

#include "ztl/tree.h"
#include "boost/serialization/ztltree.h"


using namespace ZTL;

template<typename T, bool leaf = is_leaf<T>::value>
struct traversal_helper
{
	void operator() (T const& t) {
		for (size_t ii = 0; ii<t.size; ++ii) {
			typename T::child const& x = t[ii];
			traversal_helper<typename T::child> th;
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
	ASSERT_EQ(4u,   stree.size);
	ASSERT_EQ(2u,   stree[0].size);
	ASSERT_EQ(3u,   stree[0][0].size);
	ASSERT_EQ(100u, stree[0][0][0].size);

	ASSERT_EQ(0u,   stree.level);
	ASSERT_EQ(1u,   stree[0].level);
	ASSERT_EQ(2u,   stree[0][0].level);
	ASSERT_EQ(3u,   stree[0][0][0].level);
}

TEST(SimpleTree, Initialization) {
	simpletree_type st(42, 'A', 'B', 3.141);

	ASSERT_EQ( 42,   st[0].value());
	ASSERT_EQ('A',   st[3][1].value());
	ASSERT_EQ('B',   st[3][1][2].value());
}

TEST(SimpleTree, Traits) {
	ASSERT_EQ(true, is_root<simpletree_type>::value);
	ASSERT_EQ(0, is_leaf<simpletree_type>::value);

	ASSERT_EQ(0, is_root<typename simpletree_type::child::child::child::child>::value);
	ASSERT_EQ(true, is_leaf<typename simpletree_type::child::child::child::child>::value);
}

TEST(SimpleTree, Traversal) {
	// down-ward travaersal
	traversal_helper<simpletree_type> th;
	th(stree);

	simpletree_type st(42, 'A', 'B', 3.141);
	for (size_t ii=0; ii < st.size; ii++) {
		ASSERT_EQ(42, st[ii].value());
	}
	for (size_t ii=0; ii < st[0][0].size; ii++) {
		ASSERT_EQ('B', st[0][0][ii].value());
	}
	for (size_t ii=0; ii < st[0][0][0].size; ii++) {
		ASSERT_EQ(3.141, st[0][0][0][ii].value());
	}
}

TEST(SimpleTree, Serialization) {
	std::stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << stree;

	ASSERT_TRUE(ss.str().size());
}
