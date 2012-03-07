#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

namespace ZTL {

	// ! integral type
	template<typename T, T N>
		struct integral : public std::integral_constant<T, N>
		{
			inline bool operator< (integral<T,N> const& x) { return this->value<x.value; }
			inline bool operator> (integral<T,N> const& x) { return this->value>x.value; }
			inline bool operator==(integral<T,N> const& x) { return this->value==x.value; }
			inline bool operator!=(integral<T,N> const& x) { return this->value!=x.value; }
			inline bool operator<=(integral<T,N> const& x) { return !(x.value<this->value); }
			inline bool operator>=(integral<T,N> const& x) { return !(this->value<x.value); }
		};

	// ! integer type
	template<int N>
		using int_ = integral<int, N>;

} // ZTL
