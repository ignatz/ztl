#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>

#include "ztl/array/interface.h"

#include "ztl/array/detail/standard.h"
#include "ztl/array/detail/recursive.h"
#include "ztl/array/detail/enum.h"

namespace ZTL {

template<typename T, size_t N>
using Array = ArrayInterface<detail::StandardArray<T, N>, N>;

template<typename T, size_t N>
using Enum = ArrayInterface<detail::EnumArray<T, N>, N>;

template<typename T, size_t N>
using ArrayA = ArrayInterface<detail::RecursiveArray<T, N>, N>;

} // namespace ZTL
