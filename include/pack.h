#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "integral.h"

#include <cstdlib>
#include <functional>

namespace ZTL {

template<typename ... Args> struct stack {};



template<typename ... Args>
struct size
{
	enum : size_t { value = sizeof...(Args) };
};

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
	size_t N, typename Stack = stack<>>
struct copy_n;

template<template<typename...> class Source, template<typename...> class Target,
	size_t N, typename Arg0, typename ... Args, typename ... Stack>
struct copy_n<Source<Arg0, Args...>, Target, N, stack<Stack...>>
{
	typedef typename copy_n<
			Source<Args...>, Target, N-1, stack<Stack..., Arg0>
		>::type type;
};

template<template<typename...> class Source, template<typename...> class Target,
	typename Arg0, typename ... Args, typename ... Stack>
struct copy_n<Source<Arg0, Args...>, Target, 0, stack<Stack...>>
{
	typedef Target<Stack...> type;
};

template<template<typename...> class Source, template<typename...> class Target,
	typename ... Stack>
struct copy_n<Source<>, Target, 0, stack<Stack...>>
{
	typedef Target<Stack...> type;
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

// TODO: use this as soon as compiler survives
//template<template<typename...> class Pack, size_t N, typename ArgLast, typename ... Args>
//struct pop_back<Pack<Args..., ArgLast>, N>
//{
//	typedef typename pop_back<Pack<Args...>, N-1>::type type;
//};

//template<template<typename...> class Pack, typename ... Args>
//struct pop_back<Pack<Args...>, 0>
//{
//	typedef Pack<Args...> type;
//};

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
struct merge<Pack<Args0...> , Pack<Args1...>>
{
	typedef Pack<Args0..., Args1...> type;
};



template <size_t N, typename ... Args> struct get;

template <size_t N, typename Arg0, typename ... Args>
struct get<N, Arg0, Args...>
{
	typedef typename get<N-1, Args...>::type type;
};

template <typename Arg0, typename ... Args>
struct get<0, Arg0, Args...>
{
	typedef Arg0 type;
};

template <size_t N, template<typename...> class Pack, typename ... Args>
struct get<N, Pack<Args...>>
{
	typedef typename get<N, Args...>::type type;
};

template <template<typename...> class Pack, typename ... Args>
struct get<0, Pack<Args...>>
{
	typedef typename get<0, Args...>::type type;
};

template<size_t N, int ... Args>
using get_c = get<N, int_<Args>...>;



template<size_t N>
struct arg
{
	template<typename Arg0, typename ... Args>
	constexpr static auto get(Arg0&&, Args&& ... args) noexcept -> decltype(arg<N-1>::get(std::forward<Args>(args)...)) {
		return arg<N-1>::get(std::forward<Args>(args)...);
	}
};

template<>
struct arg<0>
{
	template<typename Arg0, typename ... Args>
	constexpr static Arg0&& get(Arg0&& arg0, Args&& ...) noexcept {
		return std::forward<Arg0>(arg0);
	}
};



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
	enum : int { value = Predicate<Arg0, Type>::value ? Pos : find<Type, Pack<Args...>, Predicate, Pos+1>::value };
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



template<typename ReturnType, typename InputType>
inline void for_each(std::function<ReturnType (InputType)>) {}

template<typename ReturnType, typename InputType, typename Arg0, typename ... Args>
inline void for_each(std::function<ReturnType (InputType)> f, Arg0&& arg0, Args&& ... args)
{
	f(arg0);
	for_each(f, std::forward<Args>(args)...);
}

} // namespace ZTL
