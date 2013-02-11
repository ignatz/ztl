#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>
#include "ztl/pack/merge.h"
#include "ztl/pack/pop.h"

namespace ZTL {

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

} // namespace ZTL
