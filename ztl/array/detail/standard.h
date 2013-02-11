#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "ztl/type_traits.h"
#include "ztl/array/detail/base.h"

namespace ZTL {
namespace detail {

template<typename T, size_t N, size_t Idx = 0, typename = void>
struct StandardArray;

template<typename T, size_t N, size_t Idx>
struct StandardArray<T, N, Idx, typename enable_if<(Idx+1<N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr StandardArray() : BaseArray<T, N, Idx>() {}

	// invoke non-default constructors of T
	template<typename ... Args>
	constexpr StandardArray(Args&& ... args) :
		BaseArray<T, N, Idx>(args...), next(std::forward<Args>(args)...) {}

	StandardArray<T, N, Idx+1> next;
};

template<typename T, size_t N, size_t Idx>
struct StandardArray<T, N, Idx, typename enable_if<(Idx+1>=N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr StandardArray() : BaseArray<T, N, Idx>() {}

	// invoke non-default constructors of T
	template<typename ... Args>
		constexpr StandardArray(Args&& ... args) :
			BaseArray<T, N, Idx>(std::forward<Args>(args)...) {}
};

} // namespace ZTL
} // namespace detail
