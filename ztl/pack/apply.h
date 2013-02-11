#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

namespace ZTL {

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

} // namespace ZTL
