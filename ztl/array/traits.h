#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <array>
#include <ztl/type_traits.h>
#include <ztl/array/interface_fwd.h>
#include <ztl/array/detail/base.h>

namespace ZTL {

template<typename T>
struct is_array
{
	enum : bool { value = std::is_array<T>::value };
};

template<typename T, size_t N>
struct is_array<std::array<T, N>>
{
	enum : bool { value = true };
};

template<typename T, size_t N>
struct is_array<std::array<T, N> const>
{
	enum : bool { value = true };
};

template<typename T, size_t N>
struct is_array<ArrayInterface<T, N>>
{
	enum : bool { value = true };
};

template<typename T, size_t N>
struct is_array<ArrayInterface<T, N> const>
{
	enum : bool { value = true };
};



template<typename T>
struct is_array_impl
{
	enum : bool { value = false };
};

template<template<typename, size_t, size_t, typename...> class Class,
	typename T, size_t N, size_t Idx, typename ... Args>
struct is_array_impl<Class<T, N, Idx, Args...>>
{
	enum : bool { value = std::is_base_of<
			detail::BaseArray<T, N, Idx>,
			Class<T, N, Idx, Args...>
		>::value };
};


template<typename T, typename = void>
struct array_length;

template<typename T, size_t N>
struct array_length<T[N], void>
{
	enum : size_t { value = N };
};

template<template<typename, size_t> class Array, typename T, size_t N>
struct array_length<Array<T,N>,
	typename std::enable_if<is_array<Array<T,N>>::value>::type>
{
	enum : size_t { value = N };
};

} // namespace ZTL
