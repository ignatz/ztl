#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "array.h"
#include "pack.h"

namespace ZTL {

	// TODO: attach extern rt-data via inheritance
	// TODO: make tree serializable

	template<int Level, int TermLevel, int ... Args>
		struct Node
		{
			enum {
				level = Level,
				size = get_c<level, Args ...>::type::value,
			};

			typedef Node<level  , TermLevel, Args ...> const self_type;
			typedef Node<level-1, TermLevel, Args ...> const up_type;
			typedef Node<level+1, TermLevel, Args ...> const down_type;

			constexpr Node(up_type const * const p) : up(*p), down(this) {}

			up_type const & up;

			typedef Array<down_type, size> array_type;
			array_type const down;

			inline down_type const& operator[] (size_t const ii) const {
				return down[ii];
			}

			inline down_type& operator[] (size_t const ii) {
				return down[ii];
			}
		};


	// Terminal Node/Leaf
	template<int TermLevel, int ... Args>
		struct Node<TermLevel, TermLevel, Args ...>
		{
			enum {
				level  = sizeof...(Args),
				size   = 1,
			};

			typedef Node<level  , TermLevel, Args ...> const self_type;
			typedef Node<level-1, TermLevel, Args ...> const up_type;

			up_type const & up;

			constexpr Node(up_type const * const p) : up(*p) {}

		};


	template<int TermLevel, int ... Args>
		struct Node<0, TermLevel, Args...>
		{
			enum {
				level  = 0,
				size   = get_c<level, Args...>::type::value,
			};

			typedef Node<level  , TermLevel, Args ...> const self_type;
			typedef Node<level+1, TermLevel, Args ...> const down_type;

			typedef Array<down_type, size> array_type;
			array_type const down;

			constexpr Node() : down(this) {}

			inline down_type& operator[] (size_t const ii) {
				return down[ii];
			}

			inline down_type const& operator[] (size_t const ii) const {
				return down[ii];
			}
		};

	//template<int ... Args>
		//Node<0, sizeof...(Args), Args...> make_tree() {
			//return Node<0, sizeof...(Args), Args...>();
		//}

} // ZTL
