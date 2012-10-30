#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "ztl/type_traits.h"
#include "ztl/array/detail/base.h"

namespace ZTL {
namespace detail {

template<typename T, size_t N, size_t Idx = 0, typename = void>
struct RecursiveArray;

template<typename T, size_t N, size_t Idx>
struct RecursiveArray<T, N, Idx, typename enable_if<(Idx+1<N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr RecursiveArray() : BaseArray<T, N, Idx>(), next() {}
	constexpr RecursiveArray(std::initializer_list<T> const& l) :
		BaseArray<T, N, Idx>(l), next(l) {}


	template<template<typename, size_t> class Container, typename Q, size_t Size, typename ... Args,
		typename = typename enable_if<(Idx<Size)>::type>
	constexpr RecursiveArray(Container<Q, Size> const& con, Args&& ... args) :
		BaseArray<T, N, Idx>(*(con.begin()+Idx)), next(con, std::forward<Args>(args)...)
	{
		static_assert(Size + sizeof...(Args) <= N, "too many arguments");
	}

	template<template<typename, size_t> class Container, typename Q, size_t Size,
		typename = typename enable_if<(Idx>=Size)>::type>
	constexpr RecursiveArray(Container<Q, Size> const&) : RecursiveArray() {}

	template<template<typename, size_t> class Container, typename Q, size_t Size, typename Arg0,
		typename ... Args, typename = typename enable_if<(Idx>=Size)>::type>
	constexpr RecursiveArray(Container<Q, Size> const& con, Arg0&& arg0, Args&& ... args) :
		BaseArray<T, N, Idx>(std::forward<Arg0>(arg0)), next(con, std::forward<Args>(args)...) {}

	RecursiveArray<T, N, Idx+1> next;
};

template<typename T, size_t N, size_t Idx>
struct RecursiveArray<T, N, Idx, typename enable_if<(Idx+1>=N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr RecursiveArray() : BaseArray<T, N, Idx>() {}
	constexpr RecursiveArray(std::initializer_list<T> const& l) :
		BaseArray<T, N, Idx>(l) {}


	template<template<typename, size_t> class Container, typename Type, size_t Size, typename ... Args,
		typename = typename enable_if<(Idx<Size)>::type>
	constexpr RecursiveArray(Container<Type, Size> const& con) :
		BaseArray<T, N, Idx>(*(con.begin()+Idx)) {}

	template<template<typename, size_t> class Container, typename Type, size_t Size,
		typename ... Args, typename = typename enable_if<(Idx>=Size)>::type>
	constexpr RecursiveArray(Container<Type, Size> const&, Args&& ... args) :
		BaseArray<T, N, Idx>(std::forward<Args>(args)...)
	{
		static_assert(sizeof...(Args) <= 1, "too many arguments");
	}
};

} // namespace ZTL
} // namespace detail
