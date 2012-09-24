#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
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

} // namespace ZTL
