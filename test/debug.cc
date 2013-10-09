#include <gtest/gtest.h>
#include "ztl/debug.h"

using namespace ZTL;

struct qwerty {};
struct asdf {};

TEST(TypeStringTest, Basic) {
	qwerty q;
	ASSERT_EQ("qwerty", ZTL::typestring<qwerty>());
	ASSERT_EQ("qwerty", ZTL::typestring(q));

	asdf a;
	ASSERT_EQ("asdf", ZTL::typestring<asdf>());
	ASSERT_EQ("asdf", ZTL::typestring(a));
}
