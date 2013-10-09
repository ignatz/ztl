#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cxxabi.h>
#include <typeinfo>
#include <string>
#include <memory>

namespace {

struct FreeCString
{
	void operator () (char* s) const
	{
		free(s);
	};
};

}

namespace ZTL {

template <typename T>
inline std::string typestring(T t)
{
	std::unique_ptr<char, FreeCString> name(
		abi::__cxa_demangle(typeid(t).name(), nullptr, nullptr, nullptr));
	return std::string(name.get());
}

template <typename T>
inline std::string typestring()
{
	std::unique_ptr<char, FreeCString> name(
		abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr));
	return std::string(name.get());
}

} // namespace ZTL
