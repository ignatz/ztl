#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdint>
#include <functional>
#include <type_traits>

#include "ztl/integral.h"

namespace ZTL {

using std::size_t;

template<typename ... Args>      struct pack {};
template<typename T, T ... Args> struct pack_c {};



template<typename NoPack> struct size;

template<template<typename...> class Pack, typename ... Args>
struct size<Pack<Args...>>
{
	enum : size_t { value = sizeof...(Args) };
};



template<typename NoPack, template<typename...> class Target> struct copy;

template<template<typename...> class Source, template<typename...> class Target, typename ... Args>
struct copy<Source<Args...>, Target>
{
	typedef Target<Args...> type;
};



template<typename NoPack, template<typename...> class Target,
	size_t N, typename Pack = pack<>>
struct copy_n;

template<template<typename...> class Source, template<typename...> class Target,
	size_t N, typename Arg0, typename ... Args, typename ... Pack>
struct copy_n<Source<Arg0, Args...>, Target, N, pack<Pack...>>
{
	typedef typename copy_n<
			Source<Args...>, Target, N-1, pack<Pack..., Arg0>
		>::type type;
};

template<template<typename...> class Source, template<typename...> class Target,
	typename Arg0, typename ... Args, typename ... Pack>
struct copy_n<Source<Arg0, Args...>, Target, 0, pack<Pack...>>
{
	typedef Target<Pack...> type;
};

template<template<typename...> class Source, template<typename...> class Target,
	typename ... Pack>
struct copy_n<Source<>, Target, 0, pack<Pack...>>
{
	typedef Target<Pack...> type;
};



template<typename NoPack, size_t N = 1> struct pop_front;

template<template<typename...> class Pack, size_t N, typename Arg0, typename ... Args>
struct pop_front<Pack<Arg0, Args...>, N>
{
	typedef typename pop_front<Pack<Args...>, N-1>::type type;
};

template<template<typename...> class Pack, typename Arg0, typename ... Args>
struct pop_front<Pack<Arg0, Args...>, 0>
{
	typedef Pack<Arg0, Args...> type;
};

// silent failure, when poping from empty pack. desireable?
template<template<typename...> class Pack>
struct pop_front<Pack<>, 0>
{
	typedef Pack<> type;
};



template<typename NoPack, size_t N = 1> struct pop_back;

template<template<typename...> class Pack, size_t N, typename ... Args>
struct pop_back<Pack<Args...>, N>
{
	typedef typename copy_n<Pack<Args...>, Pack, sizeof...(Args)-N>::type type;
};



template<typename NoPack, typename ... ToPush> struct push_front;

template<template<typename...> class Pack, typename ... ToPush, typename ... Args>
struct push_front<Pack<Args...>, ToPush...>
{
	typedef Pack<ToPush..., Args...> type;
};



template<typename NoPack, typename ... ToPush> struct push_back;

template<template<typename...> class Pack, typename ... ToPush, typename ... Args>
struct push_back<Pack<Args...>, ToPush...>
{
	typedef Pack<Args..., ToPush...> type;
};



template<typename NoPack0, typename NoPack1> struct merge;

template<template<typename...> class Pack, typename ... Args0, typename ... Args1>
struct merge<Pack<Args0...>, Pack<Args1...>>
{
	typedef Pack<Args0..., Args1...> type;
};



template<size_t N, typename NoPack> struct get;

template<size_t N, template<typename...> class Pack, typename Arg0, typename ... Args>
struct get<N, Pack<Arg0, Args...>>
{
	typedef typename get<N-1, Pack<Args...>>::type type;
};

template<template<typename...> class Pack, typename Arg0, typename ... Args>
struct get<0, Pack<Arg0, Args...>>
{
	typedef Arg0 type;
};

template<size_t N, int ... Values>
using get_c = get<N, pack<int_<Values>...>>;



template<typename NoPack, size_t N = 1> struct shift_left;

template<template<typename...> class Pack, size_t N, typename ... Args>
struct shift_left<Pack<Args...>, N>
{
	enum : size_t { value = N % sizeof...(Args) };
	typedef typename merge<
			typename pop_front<Pack<Args...>, value>::type,
			typename pop_back<Pack<Args...>, sizeof...(Args)-value>::type
		>::type type;
};



template<typename NoPack, size_t N = 1> struct shift_right;

template<template<typename...> class Pack, size_t N, typename ... Args>
struct shift_right<Pack<Args...>, N>
{
	enum : size_t { value = N % sizeof...(Args) };
	typedef typename merge<
			typename pop_front<Pack<Args...>, sizeof...(Args)-value>::type,
			typename pop_back<Pack<Args...>, value>::type
		>::type type;
};



template<typename Type, typename NoPack,
	template<typename, typename> class Predicate = std::is_same, int Pos = 0> struct find;

template<typename Type, template<typename...> class Pack,
	template<typename, typename> class Predicate, int Pos, typename Arg0, typename ... Args>
struct find<Type, Pack<Arg0, Args...>, Predicate, Pos>
{
	enum : int { value = Predicate<Arg0, Type>::value ? \
					Pos : find<Type, Pack<Args...>, Predicate, Pos+1>::value };
};

template<typename Type, template<typename...> class Pack,
	template<typename, typename> class Predicate, int Pos>
struct find<Type, Pack<>, Predicate, Pos>
{
	enum : int { value = -1 };
};



template<template<typename> class F, typename NoPack> struct Apply;

template<template<typename> class F, template<typename...> class Pack, typename ... Args>
struct Apply<F, Pack<Args...>>
{
	typedef Pack<typename F<Args>::type ...> type;
};

template<template<int> class F, typename NoPack> struct Apply_c;

template<template<int> class F, template<int ...> class Pack, int ... Args>
struct Apply_c<F, Pack<Args...>>
{
	typedef Pack<F<Args>::value ...> type;
};



template<int Stop, int Start = 0, int Step = 1,
	typename NoPack = pack<>, typename = void>
struct range;

template<int Stop, int Start, int Step,
	template<typename ...> class Pack, typename ... Args>
struct range<Stop, Start, Step, Pack<Args...>,
	typename std::enable_if<(Stop>Start)>::type>
{
	typedef typename range<
			Stop, Start+Step, Step,
			typename push_back<Pack<Args...>, int_<Start>>::type
		>::type type;
};

template<int Stop, int Start, int Step,
	template<typename ...> class Pack, typename ... Args>
struct range<Stop, Start, Step, Pack<Args...>,
	typename std::enable_if<(Start>=Stop)>::type>
{
	typedef Pack<Args...> type;
};

} // namespace ZTL
