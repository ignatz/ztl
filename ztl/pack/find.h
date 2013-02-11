#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

namespace ZTL {

template<typename Type, typename NoPack,
	template<typename, typename> class Predicate = std::is_same, int Pos = 0> struct find;

template<typename Type, template<typename...> class Pack,
	template<typename, typename> class Predicate, int Pos, typename Arg0, typename ... Args>
struct find<Type, Pack<Arg0, Args...>, Predicate, Pos>
{
	enum : int { value = Predicate<Arg0, Type>::value ? \
					Pos : find<Type, Pack<Args...>, Predicate, Pos+1>::value };
};

template<typename Type, template<typename...> class Pack,
	template<typename, typename> class Predicate, int Pos>
struct find<Type, Pack<>, Predicate, Pos>
{
	enum : int { value = -1 };
};

} // namespace ZTL
