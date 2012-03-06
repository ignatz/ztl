#include <gtest/gtest.h>

#include "tree.h"

using ZTL::Node;

TEST(Tree, BasicCheck) {
	Node<0,4,2,3,1,11> tree;

	ASSERT_EQ(2, tree.size);
	ASSERT_EQ(3, tree.down[0].size);
	ASSERT_EQ(1, tree.down[0].down[0].size);
	ASSERT_EQ(11, tree.down[0].down[0].down[0].size);

	ASSERT_EQ(0, tree.level);
	ASSERT_EQ(1, tree.down[0].level);
	ASSERT_EQ(2, tree.down[0].down[0].level);
	ASSERT_EQ(3, tree.down[0].down[0].down[0].level);
}
