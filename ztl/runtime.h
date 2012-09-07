#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

#include "enable_if.h"
#include "pack.h"

namespace ZTL {

template<size_t N, typename Arg0, typename ... Args,
	typename = typename enable_if<!N>::type>
constexpr typename get<0, pack<Arg0, Args...>>::type
arg(Arg0&& arg0, Args&& ...) noexcept
{
	return std::forward<Arg0>(arg0);
}

template<size_t N, typename Arg0, typename ... Args,
	typename = typename enable_if<(N>0)>::type>
constexpr typename get<N, pack<Arg0, Args...>>::type
arg(Arg0 &&, Args&& ... args) noexcept
{
	return arg<N-1>(std::forward<Args>(args)...);
}



template<typename ReturnType, typename InputType>
inline void for_each(std::function<ReturnType (InputType)>) {}

template<typename ReturnType, typename InputType, typename Arg0, typename ... Args>
inline void for_each(std::function<ReturnType (InputType)> f, Arg0&& arg0, Args&& ... args)
{
	f(arg0); for_each(f, std::forward<Args>(args)...);
}

} // namepsace ZTL
