#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>
#include <tuple>

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


template<typename T, typename U, typename ... Args,
	template<typename ...> class Range, typename ... Ns>
auto transform_helper(
	std::tuple<Args...> const& t,
	std::function<T (U)> const& f,
	Range<Ns...>)
	-> decltype(std::make_tuple(f(std::get<Ns::value>(t))...))
{
	return std::forward_as_tuple(f(std::get<Ns::value>(t))...);
}

template<typename ... Returns, typename ... Args,
	template<typename ...> class Range, typename ... Ns>
std::tuple<Returns...>
transform_helper(
	std::tuple<Args...> const& t,
	std::tuple<std::function<Returns(Args)>...> const& f,
	Range<Ns...>)
{
	return std::forward_as_tuple(std::get<Ns::value>(f)(std::get<Ns::value>(t))...);
}


template<typename ... Returns, typename ... Args>
std::tuple<Returns...> transform(
	std::tuple<Args...> const& t,
	std::tuple<std::function<Returns(Args)>...> const& f)
{
	return transform_helper(t, f, typename range<size<std::tuple<Args...>>::value>::type {});
}

template<typename T, typename U, typename ... Args>
auto transform(
	std::tuple<Args...> const& t,
	std::function<T (U)> const& f)
	-> decltype(transform_helper(t, f, typename range<size<std::tuple<Args...>>::value>::type {}))
{
	return transform_helper(t, f, typename range<size<std::tuple<Args...>>::value>::type {});
}


template<typename ... T> inline void unpack_fun(T&& ...) {}

} // namepsace ZTL
