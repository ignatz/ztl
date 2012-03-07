#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

namespace ZTL {

	// ! integral type
	template<typename T, T N>
		struct integral
		{
			static constexpr T value = N;

			inline bool operator< (integral<T,N> const& x) { return value<x.value; }
			inline bool operator> (integral<T,N> const& x) { return value>x.value; }
			inline bool operator==(integral<T,N> const& x) { return value==x.value; }
			inline bool operator!=(integral<T,N> const& x) { return value!=x.value; }
			inline bool operator<=(integral<T,N> const& x) { return !(x.value<value); }
			inline bool operator>=(integral<T,N> const& x) { return !(value<x.value); }

			inline operator T () { return value; }
		};

	template<typename T, T N>
		constexpr T integral<T, N>::value;

	// ! integer type
	template<int N>
		using int_ = integral<int, N>;


} // ZTL
