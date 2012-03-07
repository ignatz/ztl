#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "integer.h"

#include <cstdlib>

namespace ZTL {

	template<typename NoPack, template<typename...> class Target>
		struct Copy {};

	template<template<typename...> class Source, template<typename...> class Target, typename ... Args>
		struct Copy<Source<Args...>, Target>
		{
			typedef Target<Args...> type;
		};



	template<typename NoPack> struct PopFront {};

	template<template<typename...> class Pack, typename Arg0, typename ... Args>
		struct PopFront<Pack<Arg0, Args...>>
		{
			typedef Pack<Args...> type;
		};



	template<typename NoPack, typename T> struct PushFront {};

	template<template<typename...> class Pack, typename T, typename ... Args>
		struct PushFront<T, Pack<Args...>>
		{
			typedef Pack<T, Args...> type;
		};



	template <std::size_t N, typename ... Args> struct At {};

	template <std::size_t N, typename Arg0, typename ... Args>
		struct At<N, Arg0, Args...>
		{
			typedef typename At<N-1, Args...>::type type;
		};

	template <typename Arg0, typename ... Args>
		struct At<0, Arg0, Args...>
		{
			typedef Arg0 type;
		};

	template <std::size_t N, template<typename...> class Pack, typename ... Args>
		struct At<N, Pack<Args...>>
		{
			typedef typename At<N-1, typename PopFront<Pack<Args...>>::type>::type type;
		};

	template <template<typename...> class Pack, typename Arg0, typename ... Args>
		struct At<0, Pack<Arg0, Args...>>
		{
			typedef Arg0 type;
		};

	template<size_t N, int ... Args>
		using At_c = At<N, int_<Args>...>;



	template<size_t N>
		struct Arg
		{
			template<typename Arg0, typename ... Args>
			constexpr static auto get(Arg0&&, Args&& ... args) -> decltype(Arg<N-1>::get(args...)) {
				return Arg<N-1>::get(std::forward<Args>(args)...);
			}
		};

	template<>
		struct Arg<0>
		{
			template<typename Arg0, typename ... Args>
			constexpr static Arg0 get(Arg0 arg0, Args ...) {
				return arg0;
			}
		};


} // namespace ZTL
