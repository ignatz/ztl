#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#include "ztl/tree/simple.h"
#include "ztl/type_traits.h"

namespace boost {
namespace serialization {

// normal node
template<typename Archiver, typename ... Ts, size_t ... Sizes, size_t Level, size_t TermLevel,
	typename enable = typename ZTL::enable_if<(Level != TermLevel) && Level != 0>::type>
void serialize(Archiver & ar, ZTL::SimpleTree<
		ZTL::TreeStructure<ZTL::Extend<Ts, Sizes>...>, Level, TermLevel, void> & s,
	unsigned int const)
{
	using namespace boost::serialization;
	ar & make_nvp("value", s.value())
	   & make_nvp("child", s.childs());
}

// leaf node
template<typename Archiver, typename ... Ts, size_t ... Sizes, size_t TermLevel>
void serialize(Archiver & ar, ZTL::SimpleTree<
		ZTL::TreeStructure<ZTL::Extend<Ts, Sizes>...>, TermLevel, TermLevel, void> & s,
	unsigned int const)
{
	using namespace boost::serialization;
	ar & make_nvp("value", s.value());
}

// root node
template<typename Archiver, typename ... Ts, size_t ... Sizes, size_t TermLevel>
void serialize(Archiver & ar, ZTL::SimpleTree<
		ZTL::TreeStructure<ZTL::Extend<Ts, Sizes>...>, 0, TermLevel, void> & s,
	unsigned int const)
{
	using namespace boost::serialization;
	ar & make_nvp("child", s.childs());
}

} // namespace serialization
} // namespace boost
