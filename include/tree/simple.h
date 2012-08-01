#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "tree/common.h"
#include "array.h"
#include "pack.h"
#include "enable_if.h"

namespace ZTL {

template<typename Structur, size_t Level=0, size_t TermLevel = Structur::size, typename enable = void>
struct SimpleTree;

template<typename ... Ts, size_t ... Sizes, size_t Level, size_t TermLevel>
struct SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, Level,
	TermLevel, typename enable_if<Level!=TermLevel && Level!=0>::type>
{
	enum : size_t {
		level  = Level,
		size   = get_c<level, Sizes...>::type::value,
	};

	typedef SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, level+1, TermLevel, void> child_type;
	typedef typename get<level-1,Ts...>::type value_type;

	value_type value;

	Array<child_type, size> child;

	constexpr SimpleTree() : value(), child() {}

	template<typename ... Args>
	constexpr SimpleTree(Args&& ... args) :
		value(arg<level-1>::get(args...)), child(std::forward<Args>(args)...) {}

	constexpr child_type const& operator[] (unsigned ii) {
		return child[ii];
	}
};

// root
template<typename ... Ts, size_t ... Sizes, size_t TermLevel>
struct SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, 0, TermLevel, void>
{
	enum : size_t {
		level  = 0,
		size   = get_c<level, Sizes...>::type::value,
	};

	typedef SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, level+1, TermLevel> child_type;
	typedef SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, 0, TermLevel, void> self_type;

	Array<child_type, size> child;

	constexpr SimpleTree() : child() {}

	template<typename ... Args>
	constexpr SimpleTree(Args&& ... args) : child(std::forward<Args>(args)...) {}

	constexpr child_type const& operator[] (unsigned ii) {
		return child[ii];
	}
};

// leaf
template<typename ... Ts, size_t ... Sizes, size_t TermLevel>
struct SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, TermLevel, TermLevel, void>
{
	enum : size_t {
			   level  = TermLevel,
	};

	typedef typename get<level-1,Ts...>::type value_type;

	value_type value;

	constexpr SimpleTree() : value() {}

	template<typename ... Args>
	constexpr SimpleTree(Args&& ... args) : value(arg<level-1>::get(args...)) {}
};



// trait specializations
template<typename Structur, size_t Level, size_t TermLevel>
struct is_leaf<SimpleTree<Structur, Level, TermLevel, void>>
{
	enum : bool { value = (Level!=TermLevel) ? false : true };
};

template<typename Structur, size_t Level, size_t TermLevel>
struct is_root<SimpleTree<Structur, Level, TermLevel, void>>
{
	enum : bool { value = (Level!=0) ? false : true };
};

} // namespace ZTL
