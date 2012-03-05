#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cxxabi.h>
#include <typeinfo>
#include <string>

namespace ZTL {

	template <typename T>
		inline std::string typestring(T t) {
			const std::type_info& ti = typeid(t);
			return abi::__cxa_demangle(ti.name(), nullptr, nullptr, nullptr);
		}

} // ZTL
