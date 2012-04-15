#pragma once

#include "impl.h"

#include <boost/serialization/serialization.hpp>

namespace boost {

namespace serialization {

// ArrayInterface serializer
template<typename Archiver, template<typename, size_t, size_t, typename...> class Class,
		typename T, size_t N, typename ... Args>
	void serialize(Archiver & ar, ZTL::ArrayInterface<Class<T, N, 0, Args...>, N> & s,
				   unsigned int const) { ar & s.array; }



// base class BaseArray serializer
template<typename Archiver, typename T, size_t N, size_t Idx>
	void serialize(Archiver & ar, ZTL::BaseArray<T, N, Idx>& s, unsigned int const) {
		ar & s.value;
	}



// serializers for all implementations
template<typename Archiver, template<typename, size_t, size_t, typename...> class Class,
		typename T, size_t N, size_t Idx, typename ... Args, typename = typename std::enable_if<
		ZTL::is_array_impl<Class<T, N, Idx, Args...>>::value>::type>
	typename std::enable_if<(Idx+1<N), void>::type
	serialize(Archiver & ar, Class<T, N, Idx, Args...>& s, unsigned int const) {
		ar & boost::serialization::base_object<ZTL::BaseArray<T, N, Idx>>(s);
		ar & s.next;
	}

template<typename Archiver, template<typename, size_t, size_t, typename...> class Class,
		typename T, size_t N, size_t Idx, typename ... Args, typename = typename std::enable_if<
		ZTL::is_array_impl<Class<T, N, Idx, Args...>>::value>::type>
	typename std::enable_if<(Idx+1>=N), void>::type
	serialize(Archiver & ar, Class<T, N, Idx, Args...>& s, unsigned int const) {
		ar & boost::serialization::base_object<ZTL::BaseArray<T, N, Idx>>(s);
	}

} // namespace serialization

} // namespace boost
