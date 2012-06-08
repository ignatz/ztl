#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

namespace ZTL {

// forward declarations
template<typename NoArrayType, size_t N> class ArrayInterface;

template<typename T, size_t N, size_t Idx = 0, typename = void>
struct StandardArray;

template<typename T, size_t N, size_t Idx = 0, typename = void>
struct RecursiveArray;

template<typename T, size_t N, size_t Idx = 0, typename = void>
struct EnumArray;



template<typename T, size_t N, size_t Idx>
struct BaseArray
{
	T value;

	template<typename ... Args>
	constexpr BaseArray(Args&& ... args) :
		value(std::forward<Args>(args)...) {}

	constexpr BaseArray(std::initializer_list<T> const& l) :
		value(l.begin() + Idx != l.end() ? *(l.begin()+Idx) : T()) {}
};



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
struct is_array<ArrayInterface<T, N>>
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
			BaseArray<T, N, Idx>,
			Class<T, N, Idx, Args...>
		>::value };
};

} // namespace ZTL
