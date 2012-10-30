#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/array.hpp>

#include "ztl/array.h"
#include "ztl/type_traits.h"

namespace boost {
namespace serialization {

// ArrayInterface serializer
template<typename Archiver, template<typename, size_t, size_t, typename...> class Class,
typename T, size_t N, typename ... Args>
void serialize(Archiver& ar, ZTL::ArrayInterface<Class<T, N, 0, Args...>, N>& s,
	unsigned int const)
{
	using namespace boost::serialization;
	ar & make_nvp("data", reinterpret_cast<boost::array<T, N>&>(s));
}

} // namespace serialization
} // namespace boost
