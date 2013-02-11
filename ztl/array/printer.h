#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <ostream>

#include "ztl/array.h"
#include "ztl/type_traits.h"
#include "ztl/array/range_access.h"

template<typename Array>
typename ZTL::enable_if<
	ZTL::is_array<Array>::value && (
	!std::is_array<Array>::value ||
	!std::is_same<typename std::remove_extent<Array>::type, char>::value),
	std::ostream&>::type
operator<< (std::ostream& o, Array const& s)
{
	for(auto const& val : s)
	{
		o << val;
		if(&val == std::end(s)-1)
			break;
		o << o.fill();
	}
	return o;
}
