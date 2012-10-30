#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

namespace ZTL {
namespace detail {

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

} // namespace detail
} // namespace ZTL
