#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "ztl/type_traits.h"
#include "ztl/array/detail/base.h"

namespace ZTL {
namespace detail {

template<typename T, size_t N, size_t Idx = 0, typename = void>
struct EnumArray;

template<typename T, size_t N, size_t Idx>
struct EnumArray<T, N, Idx, typename enable_if<(Idx+1<N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr EnumArray() : BaseArray<T, N, Idx>(Idx), next() {}

	template<typename ... Args>
	constexpr EnumArray(Args&& ... args) :
		BaseArray<T, N, Idx>(Idx, args...), next(std::forward<Args>(args)...) {}

	EnumArray<T, N, Idx+1> next;
};

template<typename T, size_t N, size_t Idx>
struct EnumArray<T, N, Idx, typename enable_if<(Idx+1>=N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr EnumArray() : BaseArray<T, N, Idx>(Idx) {}

	template<typename ... Args>
	constexpr EnumArray(Args&& ... args) :
		BaseArray<T, N, Idx>(Idx, args...){}
};

} // namespace ZTL
} // namespace detail
