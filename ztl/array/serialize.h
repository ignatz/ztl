#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/array.hpp>

#include "ztl/array/impl.h"
#include "ztl/array/trait.h"
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
	ar & make_nvp("array", s.array);
}


// serializers for all implementations
template<typename Archiver, template<typename, size_t, size_t, typename...> class Class,
	typename T, size_t N, size_t Idx, typename ... Args, typename = typename ZTL::enable_if<
		ZTL::is_array_impl<Class<T, N, Idx, Args...>>::value>::type>
typename ZTL::enable_if<(Idx+1<N), void>::type
serialize(Archiver& ar, Class<T, N, Idx, Args...>& s, unsigned int const)
{
	using namespace boost::serialization;
	ar & make_nvp("data", reinterpret_cast<boost::array<T, N>&>(s));
}

template<typename Archiver, template<typename, size_t, size_t, typename...> class Class,
	typename T, size_t N, size_t Idx, typename ... Args, typename = typename ZTL::enable_if<
		ZTL::is_array_impl<Class<T, N, Idx, Args...>>::value>::type>
typename ZTL::enable_if<(Idx+1>=N), void>::type
serialize(Archiver&, Class<T, N, Idx, Args...>&, unsigned int const)
{}

} // namespace serialization
} // namespace boost
