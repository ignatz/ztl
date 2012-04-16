#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "integral.h"

#include <cstdlib>
#include <functional>

namespace ZTL {

	template<typename ... Args>     struct stack {};
	template<typename T, T ... Seq> struct sequence {};



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

	template<typename T, template<typename, T...> class Sequence, T ... Args>
		struct size<Sequence<T, Args...>>
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
			//typedef typename pop_back<Pack<Args...>, N-1>::type type;
		//};

	//template<template<typename...> class Pack, typename ... Args>
		//struct pop_back<Pack<Args...>, 0>
		//{
			//typedef Pack<Args...> type;
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

	template <size_t N>
		struct get<N>
		{
			template<typename Arg0, typename ... Args>
				constexpr static auto arg(Arg0&&, Args&& ... args) noexcept ->
				decltype(get<N-1>::arg(std::forward<Args>(args)...)) {
					return get<N-1>::arg(std::forward<Args>(args)...);
				}
		};

	template<>
		struct get<0>
		{
			template<typename Arg0, typename ... Args>
				constexpr static Arg0&& arg(Arg0&& arg0, Args&& ...) noexcept {
					return std::forward<Arg0>(arg0);
				}
		};

	template<size_t N, int ... Args>
		using get_c = get<N, int_<Args>...>;



	template<typename T, T From, T To, template<typename,T...> class SeqType = sequence,
		typename Seq = SeqType<T>, typename = void>
		struct make_sequence;

	template<typename T, T From, T To, template<typename,T...> class SeqType, T ... S>
		struct make_sequence<T, From, To, SeqType, SeqType<T, S...>, typename std::enable_if<(From<To)>::type>
		{
			typedef typename make_sequence<
				T, From+1, To, SeqType, SeqType<T, S..., From>>::type type;
		};

	template<typename T, T From, T To, template<typename,T...> class SeqType, T ... S>
		struct make_sequence<T, From, To, SeqType, SeqType<T, S...>, typename std::enable_if<(From>To)>::type>
		{
			typedef typename make_sequence<
				T, From+1, To, SeqType, SeqType<T, S..., From>>::type type;
		};

	template<typename T, int To, template<typename, T...> class SeqType, T ... S>
		struct make_sequence<T, To, To, SeqType, SeqType<T, S...>, void>
		{
			typedef SeqType<T, S...> type;
		};



	template<template<typename...> class Pack, typename NoPack>
		struct promote
		{
			typedef Pack<NoPack> type;

			static inline type get(NoPack&& nt) {
				return type(std::forward<NoPack>(nt));
			}
		};

	template<template<typename...> class Pack, typename ... Type>
		struct promote<Pack, Pack<Type...>>
		{
			typedef Pack<Type...> type;

			static inline type get(type&& t) { return t; }
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



	template<typename Type, typename NoPack, template<typename, typename> class Predicate = std::is_same, int Pos = 0> struct find;

	template<typename Type, template<typename...> class Pack, template<typename, typename> class Predicate, int Pos, typename Arg0, typename ... Args>
		struct find<Type, Pack<Arg0, Args...>, Predicate, Pos>
		{
			enum : int { value = Predicate<Arg0, Type>::value ? Pos : find<Type, Pack<Args...>, Predicate, Pos+1>::value };
		};

	template<typename Type, template<typename...> class Pack, template<typename, typename> class Predicate, int Pos>
		struct find<Type, Pack<>, Predicate, Pos>
		{
			enum : int { value = -1 };
		};



	template<template<int, typename> class F, typename NoPack,
		typename Seq = typename make_sequence<int, 0, size<NoPack>::value>::type> struct Apply;

	template<template<int, typename> class F, template<typename...> class Pack, typename ... Args, int ... Enum>
		struct Apply<F, Pack<Args...>, sequence<int, Enum...>>
		{
			typedef Pack<typename F<Enum, Args>::type ...> type;
		};

	template<template<int, int> class F, typename T, typename NoSequence,
		typename Seq = typename make_sequence<T, 0, size<NoSequence>::value>::type> struct Apply_c;

	template<template<int, int> class F, typename T, template<typename, T...> class Sequence, T ... Args, T ... Seq>
		struct Apply_c<F, T, Sequence<T, Args...>, sequence<T, Seq...>>
		{
			typedef Sequence<T, F<Seq, Args>::value ...> type;
		};



	template<typename ReturnType, typename InputType>
		inline void for_each(std::function<ReturnType (InputType)>) {}

	template<typename ReturnType, typename InputType, typename Arg0, typename ... Args>
		inline void for_each(std::function<ReturnType (InputType)> f, Arg0&& arg0, Args&& ... args) {
			f(arg0);
			for_each(f, std::forward<Args>(args)...);
		}

} // namespace ZTL
