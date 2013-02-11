#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>
#include "ztl/pack/copy.h"

namespace ZTL {

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

} // namespace ZTL
