#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer


namespace ZTL {

template<typename T, size_t Size>
struct Extend {};

template<typename ... Extends>
struct TreeStructure;

template<typename ... Ts, size_t ... Sizes>
struct TreeStructure<Extend<Ts, Sizes>...> {
	enum : size_t { size = sizeof...(Ts) };
};



// traits
template<typename T>
struct is_leaf {};

template<typename T>
struct is_root {};

} // namespace ZTL
