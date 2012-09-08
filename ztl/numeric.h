#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdint>
#include <type_traits>

namespace ZTL {

using std::size_t;

template<typename T, typename C>
struct integral_unpack;

template<typename T, template<typename, T> class C, T N>
struct integral_unpack<T, C<T,N>>
{
	enum : T { value = N };
};



template<int N>
struct positive {
	enum : int { value = N };
};

template<int N>
using negative = positive<-N>;



template<int ... N>
struct add{
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



template<typename T, T N, T M>
struct eq_t {
	enum : bool { value = N == M };
};

template<typename T, T N, T M>
struct un_eq_t {
	enum : bool { value = N != M };
};

template<typename T, T N, T M>
struct less_t {
	enum : bool { value = N < M };
};

template<typename T, T N, T M>
struct less_eq_t {
	enum : bool { value = N <= M };
};

template<typename T, T N, T M>
struct more_t {
	enum : bool { value = N > M };
};

template<typename T, T N, T M>
struct more_eq_t {
	enum : bool { value = N >= M };
};

template<int N, int M> using eq      = eq_t<int, N, M>;
template<int N, int M> using un_eq   = un_eq_t<int, N, M>;
template<int N, int M> using less    = less_t<int, N, M>;
template<int N, int M> using less_eq = less_eq_t<int, N, M>;
template<int N, int M> using more    = more_t<int, N, M>;
template<int N, int M> using more_eq = more_eq_t<int, N, M>;



template<typename T, template<typename, T, T> class C, T N, T ... M>
struct extremum_t {
	enum : T { value = C<T, N, extremum_t<T, C, M...>::value>::value ? \
	   N : extremum_t<T, C, M...>::value };
};

template<typename T, template<typename, T, T> class C, T N>
struct extremum_t<T, C, N> {
	enum : T { value = N };
};

template<int N, int ... M>
using max = extremum_t<int, more_t, N, M...>;

template<int N, int ... M>
using min = extremum_t<int, less_t, N, M...>;



template<intmax_t Base, intmax_t Exp>
struct pow {
	enum : intmax_t { value = Base * pow<Base, Exp-1>::value };
};

template<intmax_t Base>
struct pow<Base, 0> {
	enum : intmax_t { value = 1 };
};



template<size_t N>
struct factorial
{
	enum : size_t { value = N * factorial<N-1>::value };
};

template<>
struct factorial<0>
{
	enum : size_t { value = 1 };
};



template<size_t N, size_t II = sizeof(size_t)*8>
struct log2
{
	static_assert(N!=0, "log2<0> not defined");
	enum : size_t { value = std::conditional<
		   (N >> (II-1)) & 0x1,
		   std::integral_constant<size_t, II-1>,
		   log2<N, II-1>
		>::type::value };
};

template<size_t N>
struct log2<N, 1>
{
	enum : size_t { value = 0 };
};



template<size_t N>
struct fibonacci
{
	enum : size_t { value = fibonacci<N-1>::value + fibonacci<N-2>::value };
};

template<> struct fibonacci<1> { enum : size_t { value = 1 }; };

template<> struct fibonacci<0> { enum : size_t { value = 0 }; };

} // namespace ZTL
