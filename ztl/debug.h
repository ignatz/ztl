#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cxxabi.h>
#include <typeinfo>
#include <string>
#include <memory>

namespace ZTL {

inline std::string demangle(std::string const& n)
{
	int status {};
	std::unique_ptr<char[], decltype(&free)> name(
		abi::__cxa_demangle(n.c_str(), nullptr, 0, &status), &free);
	return status==0 ? name.get() : "__cxa_demangle error";
}

template <typename T>
inline std::string typestring(T t)
{
	return demangle(typeid(t).name());
}

template <typename T>
inline std::string typestring()
{
	return demangle(typeid(T).name());
}

} // namespace ZTL
