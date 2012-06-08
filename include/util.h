#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <boost/serialization/serialization.hpp>

namespace ZTL {

// ! declare variables to remain unused
template<typename T>
inline void unused(T t)
{
	static_cast<void>(t);
}



template<typename T>
struct Trait
{
	typedef T type;
};



template<typename ... Args>
class Void
{
public:
	template<typename ... Ts>
	constexpr Void(Ts&& ...) {}

private:
	friend class boost::serialization::access;
	template<typename Archiver>
	void serialize(Archiver&, unsigned int const) {}
};

} // ZTL
