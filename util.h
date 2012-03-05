#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "numeric.h"
#include "integer.h"

#include <boost/serialization/serialization.hpp>

namespace ZTL {

	// ! accessed Nth type element of ... Args
	template<typename N, typename T, typename ... Args>
		struct arg
		{
			typedef typename arg<typename dec<N>::type, Args ... >::type type;

			constexpr arg(T t __attribute__((unused)), Args... args) : m(args...), value(m.value) {}
			constexpr arg() = delete;

			arg<typename dec<N>::type, Args ... > m;
			type value;
		};

	template<typename T, typename ... Args>
		struct arg<int_<0>, T, Args...>
		{
			typedef T type;

			constexpr arg(T t, Args... args __attribute__((unused))) : value(t) {}
			constexpr arg() = delete;
			type value;
		};


	template<int N, typename ... Args>
		using arg_c = arg<int_<N>, Args...>;


	template<typename N, int T, int ... Args>
		struct argv {
			typedef typename argv<typename dec<N>::type, Args ... >::type type;
		};

	template<int T, int ... Args>
		struct argv<int_<0>, T, Args...>
		{
			typedef int_<T> type;
		};

	template<int N, int ... Args>
		using argv_c = argv<int_<N>, Args...>;




	// ! get type of Nth element of some data type
	template<typename N, typename T>
		struct at;

	template<int N, typename T>
		using at_c = at<int_<N>, T>;



	// base size implementations
	template<typename  T>
		struct size {};



	// statically ensure non-constness of types
	template<typename T>
		struct deconst_
		{
			typedef T type;
		};

	template<typename T>
		struct deconst_<T const>
		{
			typedef T type;
		};

	// statically ensure constness of types
	template<typename T>
		struct const_
		{
			typedef T const type;
		};

	template<typename T>
		struct const_<T const>
		{
			typedef T type;
		};



	// ! flag unused arguments
	template<typename T>
		inline void unused(T t) {
			static_cast<void>(t);
		}



	template<typename T>
		struct Trait
		{
			typedef T type;
		};



	template<typename ... Args>
		struct Void
		{
			constexpr Void(Args ... args __attribute__((unused)) ) {}

			private:
			friend class boost::serialization::access;
			template<typename Archiver>
				void serialize(Archiver& ar, unsigned int const version) {
					//unused(version);
					//unused(ar);
				}
		};



	template<bool Expression, typename True, typename False>
		struct If
		{
			typedef True type;
		};

	template<typename True, typename False>
		struct If<false, True, False>
		{
			typedef False type;
		};

} // ZTL
