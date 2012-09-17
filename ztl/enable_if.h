#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

#include "ztl/pack.h"
#include "ztl/integral.h"
#include "ztl/numeric.h"

namespace ZTL {

template<bool Predicate, typename T = void>
using enable_if = std::enable_if<Predicate, T>;

template<bool Predicate, typename T = void>
using disable_if = enable_if<!Predicate, T>;

template<typename T, typename ... Args>
using any_of = enable_if<find<T, stack<Args...>>::value >= 0, void>;

template<typename T, typename ... Args>
using non_of = enable_if<find<T, stack<Args...>>::value == -1, void>;

template<typename T, typename ... Args>
using is_same = bool_<add<std::is_same<T, Args>::value...>::value>;

template<typename T, typename ... Args>
using all_of = enable_if<is_same<T, Args...>::value>;

} // namespace ZTL
