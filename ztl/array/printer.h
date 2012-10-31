#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <ostream>

#include "ztl/array.h"
#include "ztl/type_traits.h"

template<template<typename, size_t> class Array, typename Type, size_t N,
	typename = typename ZTL::enable_if<ZTL::is_array<Array<Type, N>>::value>::type>
std::ostream& operator<< (std::ostream& o, Array<Type, N> const& s)
{
	o << "[";
	for(size_t ii = 0; ii<s.size(); ++ii)
		o << s[ii] << ((ii < s.size()-1) ? "," : "");
	o << "]";
	return o;
}
