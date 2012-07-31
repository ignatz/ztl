#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

#include "pack.h"

namespace ZTL {

template<bool Predicate, typename T = void>
using enable_if = std::enable_if<Predicate, T>;

template<bool Predicate, typename T = void>
using disable_if = std::enable_if<!Predicate, T>;

template<typename T, typename ... Args>
using any_of = std::enable_if<find<T, stack<Args...>>::value >= 0, void>;

template<typename T, typename ... Args>
using non_of = std::enable_if<find<T, stack<Args...>>::value == -1, void>;

} // namespace ZTL
