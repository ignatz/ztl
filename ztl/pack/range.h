#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>

#include "ztl/pack/pack.h"
#include "ztl/pack/push.h"

namespace ZTL {

template<int Stop, int Start = 0, int Step = 1,
	typename NoPack = pack<>, typename = void>
struct range;

template<int Stop, int Start, int Step,
	template<typename ...> class Pack, typename ... Args>
struct range<Stop, Start, Step, Pack<Args...>,
	typename std::enable_if<(Stop>Start)>::type>
{
	typedef typename range<
			Stop, Start+Step, Step,
			typename push_back<Pack<Args...>,
			std::integral_constant<int, Start>>::type
		>::type type;
};

template<int Stop, int Start, int Step,
	template<typename ...> class Pack, typename ... Args>
struct range<Stop, Start, Step, Pack<Args...>,
	typename std::enable_if<(Start>=Stop)>::type>
{
	typedef Pack<Args...> type;
};

} // namespace ZTL
