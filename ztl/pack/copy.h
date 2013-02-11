#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>
#include "ztl/pack/pack.h"

namespace ZTL {

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

} // namespace ZTL
