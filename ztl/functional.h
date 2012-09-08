#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

#include "pack.h"

namespace ZTL {

template <typename Object, typename ReturnType, typename... Args>
inline std::function<ReturnType(Args...)> bind_mf(ReturnType(Object::*mem_ptr)(Args ...), Object& obj)
{
	return [&,mem_ptr](Args&& ... args) -> ReturnType {
		return (obj.*mem_ptr)(std::forward<Args>(args)...);
	};
}

} // namespace ZTL
