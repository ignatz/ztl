#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cxxabi.h>
#include <typeinfo>
#include <string>

namespace ZTL {

template <typename T>
inline std::string typestring(T t)
{
	char * name = abi::__cxa_demangle(ti.name(), nullptr, nullptr, nullptr);
	std::string ret(name);
	free(name);
	return ret;
}

template <typename T>
inline std::string typestring()
{
	char * name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
	std::string ret(name);
	free(name);
	return ret;
}

} // namespace ZTL
