#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>
#include <type_traits>

#include "ztl/numeric.h"

namespace ZTL {

// some integral type aliases
template<typename T, int N>
using ic_ = std::integral_constant<T, N>;

template<int N>
using int_ = std::integral_constant<int, N>;

template<size_t N>
using size_t_ = std::integral_constant<size_t, N>;

template<bool N>
using bool_ = std::integral_constant<bool, N>;



// instance comparisons (you shouldn't instantiate constants anyway)
template<typename T, T N, T M>
constexpr bool operator == (ic_<T,N>, ic_<T,M>)
{
	return eq_t<T, N, M>::value;
}

template<typename T, T N, T M>
constexpr bool operator != (ic_<T,N>, ic_<T,M>)
{
	return un_eq_t<T, N, M>::value;
}

template<typename T, T N, T M>
constexpr bool operator > (ic_<T,N>, ic_<T,M>)
{
	return more_t<T, N, M>::value;
}

template<typename T, T N, T M>
constexpr bool operator >= (ic_<T,N>, ic_<T,M>)
{
	return more_eq_t<T, N, M>::value;
}

template<typename T, T N, T M>
constexpr bool operator < (ic_<T,N>, ic_<T,M>)
{
	return less_t<T, N, M>::value;
}

template<typename T, T N, T M>
constexpr bool operator <= (ic_<T,N>, ic_<T,M>)
{
	return less_eq_t<T, N, M>::value;
}

} // namespace ZTL
