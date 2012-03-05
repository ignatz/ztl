#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "integer.h"
#include "array.h"
#include "util.h"

namespace ZTL {

	// TODO: attach extern rt-data via inheritance

	template<int Level, int TermLevel, int ... Args>
		struct node
		{
			enum {
				level = Level,
				size = argv_c<level, Args ...>::type::value,
			};

			typedef node<level  , TermLevel, Args ...> const self_type;
			typedef node<level-1, TermLevel, Args ...> const up_type;
			typedef node<level+1, TermLevel, Args ...> const down_type;

			constexpr node(up_type const * const p) : up(p), down(this) {}

			up_type const * const up;

			typedef Array<down_type, size> array_type;
			array_type const down;

			inline down_type const& operator[] (idx_t ii) const {
				return down[ii];
			}

			inline down_type& operator[] (idx_t ii) {
				return down[ii];
			}
		};


	// Terminal node/Leaf
	template<int TermLevel, int ... Args>
		struct node<TermLevel, TermLevel, Args ...>
		{
			enum {
				level  = sizeof...(Args),
				size   = 1,
			};

			typedef node<level  , TermLevel, Args ...> const self_type;
			typedef node<level-1, TermLevel, Args ...> const up_type;

			up_type const * const up;

			constexpr node(up_type const * const p) : up(p) {}

		};


	template<int TermLevel, int ... Args>
		struct node<0, TermLevel, Args...>
		{
			enum {
				level  = 0,
				size   = argv_c<level, Args...>::type::value,
			};

			typedef node<level  , TermLevel, Args ...> const self_type;
			typedef node<level+1, TermLevel, Args ...> const down_type;

			typedef Array<down_type, size> array_type;
			array_type const down;

			constexpr node() : down(this) {}

			inline down_type& operator[] (idx_t ii) {
				return down[ii];
			}

			inline down_type const& operator[] (idx_t ii) const {
				return down[ii];
			}
		};

	template<int ... Args>
		struct tree
		{
			constexpr tree() : t() {}
			node<0, sizeof...(Args), Args...> t;
		};


} // ZTL
