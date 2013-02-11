#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>

namespace ZTL {

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
struct get_c : public get<N, pack<std::integral_constant<int, Values>...>> {};

} // namespace ZTL
