#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <boost/serialization/serialization.hpp>

#include "ztl/tree/simple.h"
#include "ztl/enable_if.h"

namespace boost {
namespace serialization {

// normal node
template<typename Archiver, typename ... Ts, size_t ... Sizes, size_t Level, size_t TermLevel,
	typename enable = typename ZTL::enable_if<(Level != TermLevel) && Level != 0>::type>
void serialize(Archiver & ar, ZTL::SimpleTree<
		ZTL::TreeStructure<ZTL::Extend<Ts, Sizes>...>, Level, TermLevel, void> & s,
	unsigned int const)
{
	ar & s.value;
	ar & s.child;
}

// leaf node
template<typename Archiver, typename ... Ts, size_t ... Sizes, size_t TermLevel>
void serialize(Archiver & ar, ZTL::SimpleTree<
		ZTL::TreeStructure<ZTL::Extend<Ts, Sizes>...>, TermLevel, TermLevel, void> & s,
	unsigned int const)
{
	ar & s.value;
}

// root node
template<typename Archiver, typename ... Ts, size_t ... Sizes, size_t TermLevel>
void serialize(Archiver & ar, ZTL::SimpleTree<
		ZTL::TreeStructure<ZTL::Extend<Ts, Sizes>...>, 0, TermLevel, void> & s,
	unsigned int const)
{
	ar & s.child;
}

} // namespace serialization
} // namespace boost
