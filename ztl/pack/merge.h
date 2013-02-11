#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

namespace ZTL {

template<typename NoPack0, typename NoPack1> struct merge;

template<template<typename...> class Pack, typename ... Args0, typename ... Args1>
struct merge<Pack<Args0...>, Pack<Args1...>>
{
	typedef Pack<Args0..., Args1...> type;
};

} // namespace ZTL
