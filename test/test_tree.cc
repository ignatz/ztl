#include <gtest/gtest.h>

#include "tree.h"

using ZTL::Node;

Node<0,4,2,3,1,11> tree;

TEST(Tree, BasicCheck) {
	ASSERT_EQ(2, tree.size);
	ASSERT_EQ(3, tree[0].size);
	ASSERT_EQ(1, tree[0][0].size);
	ASSERT_EQ(11, tree[0][0][0].size);

	ASSERT_EQ(0, tree.level);
	ASSERT_EQ(1, tree[0].level);
	ASSERT_EQ(2, tree[0][0].level);
	ASSERT_EQ(3, tree[0][0][0].level);
}

TEST(Tree, Traversal) {
	ASSERT_EQ(&tree[0], &tree[0][0].up);
}
