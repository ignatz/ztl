#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "integer.h"
#include "cstdint"

namespace ZTL {

	template<typename T>
		struct unpack_c
		{
			enum : int { value = T::value };
		};



	template<int N>
		struct positive
		{
			enum : int { value = N };
		};

	template<int N>
		using negative = positive<-N>;



	template<int ... N>
		struct add {
			enum : int { value = 0 };
		};

	template<int N, int ... M>
		struct add<N, M...> {
			enum : int { value = N + add<M...>::value };
		};



	template<int N>
		using inc = add<N, 1>;

	template<int N>
		using dec = add<N,-1>;



	template<int N, int ... M>
		struct sub {
			enum : int { value = N - add<M...>::value };
		};



	template<int ... N>
		struct mult {
			enum : int { value = 1 };
		};

	template<int N, int ... M>
		struct mult<N, M...> {
			enum : int { value = N * mult<M...>::value };
		};



	template<int N, int ... M>
		struct div {
			enum : int { value = N / mult<M...>::value };
		};



	template<int N, int M>
		struct mod {
			enum : int { value = N % M };
		};



	template<int N, int M>
		struct less {
			enum : bool { value = N < M };
		};

	template<int N, int M>
		struct more {
			enum : bool { value = N > M };
		};



	template<template<int, int> class T, int N, int ... M>
		struct extremum {
			enum : int { value = T<N, extremum<T, M...>::value>::value ? N : extremum<T, M...>::value };
		};

	template<template<int, int> class T, int N>
		struct extremum<T, N> {
			enum : int { value = N };
		};

	template<int N, int ... M>
		using max = extremum<more, N, M...>;

	template<int N, int ... M>
		using min = extremum<less, N, M...>;



	template<intmax_t Base, intmax_t Exp>
		struct pow {
			enum : intmax_t { value = Base * pow<Base, Exp-1>::value };
		};

	template<intmax_t Base>
		struct pow<Base, 0> {
			enum : intmax_t { value = 1 };
		};



	template<uintmax_t N>
		struct factorial
		{
			enum : uintmax_t { value = N * factorial<N-1>::value };
		};

	template<>
		struct factorial<0>
		{
			enum : uintmax_t { value = 1 };
		};



	template<uintmax_t N>
		struct fibonacci
		{
			enum : uintmax_t { value = fibonacci<N-1>::value + fibonacci<N-2>::value };
		};

	template<> struct fibonacci<1> { enum : uintmax_t { value = 1 }; };

	template<> struct fibonacci<0> { enum : uintmax_t { value = 0 }; };

} // ZTL
