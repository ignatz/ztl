#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

namespace ZTL {

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

} // namespace ZTL
