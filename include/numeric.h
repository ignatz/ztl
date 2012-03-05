#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "integer.h"

namespace ZTL {

	template<typename ... N>
		struct add {
			typedef int_<0> type;
		};

	template<typename N, typename ... M>
		struct add<N, M...> {
			typedef int_<N::value + add<M...>::type::value> type;
		};

	template<int ... N>
		using add_c = add<int_<N>...>;

	template<typename N>
		using inc = add<N, int_<1>>;

	template<int N>
		using inc_c = add_c<N, 1>;




	template<typename ... N>
		struct sub {
			typedef int_<0> type;
		};

	template<typename N, typename ... M>
		struct sub<N, M...> {
			typedef int_<N::value - sub<M...>::type::value> type;
		};

	template<int ... N>
		using sub_c = sub<int_<N>...>;

	template<typename N>
		using dec = sub<N, int_<1>>;

	template<int N>
		using dec_c = sub_c<N, 1>;




	template<typename ... N>
		struct mult {
			typedef int_<1> type;
		};

	template<typename N, typename ... M>
		struct mult<N, M...> {
			typedef int_<N::value * mult<M...>::type::value> type;
		};

	template<int ... N>
		using mult_c = mult<int_<N>...>;



	template<typename ... N>
		struct div {
			typedef int_<1> type;
		};

	template<typename N, typename ... M>
		struct div<N, M...> {
			typedef int_<N::value / mult<M...>::type::value> type;
		};

	template<int ... N>
		using div_c = div<int_<N>...>;



	template<typename N, typename M>
		struct mod {
			typedef int_<N::value % M::value> type;
		};

	template<int N, int M>
		using mod_c = mod<int_<N>, int_<M>>;



	template<typename N, typename ... M>
		struct max {
			typedef int_<(N::value > max<M...>::type::value) ? N::value : max<M...>::type::value> type;
		};

	template<typename N>
		struct max<N> {
			typedef N type;
		};

	template<int ... N>
		using max_c = max<int_<N>...>;

	template<typename N, typename ... M>
		struct min {
			typedef int_<(N::value < min<M...>::type::value) ? N::value : min<M...>::type::value> type;
		};

	template<typename N>
		struct min<N> {
			typedef N type;
		};

	template<int ... N>
		using min_c = min<int_<N>...>;



	template<typename N>
		struct factorial
		{
			typedef typename mult<N, typename factorial<typename dec<N>::type>::type>::type type;
		};

	template<>
		struct factorial<int_<0>>
		{
			typedef int_<1> type;
		};

	template<int N>
		using factorial_c = factorial<int_<N>>;



	template<int N>
		struct fibonacci_c
		{
			typedef int_<fibonacci_c<N-1>::type::value + fibonacci_c<N-2>::type::value> type;
		};

	template<> struct fibonacci_c<0> { typedef int_<0> type; };

	template<> struct fibonacci_c<1> { typedef int_<0> type; };

	template<typename N>
		using fibonacci = fibonacci_c<N::value>;

} // ZTL
