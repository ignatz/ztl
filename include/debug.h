#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cxxabi.h>
#include <typeinfo>
#include <string>

namespace ZTL {

template <typename T>
inline std::string typestring(T t)
{
	std::type_info const& ti = typeid(t);
	return abi::__cxa_demangle(ti.name(), nullptr, nullptr, nullptr);
}

template <typename T>
inline std::string typestring()
{
	return abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
}

} // namespace ZTL
