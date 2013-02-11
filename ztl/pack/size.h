#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>

namespace ZTL {

template<typename NoPack> struct size;

template<template<typename...> class Pack, typename ... Args>
struct size<Pack<Args...>>
{
	enum : size_t { value = sizeof...(Args) };
};

} // namespace ZTL
