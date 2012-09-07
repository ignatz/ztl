#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "ztl/tree/common.h"
#include "ztl/array.h"
#include "ztl/pack.h"
#include "ztl/runtime.h"
#include "ztl/enable_if.h"

namespace ZTL {

template<typename Structur, size_t Level=0,
	size_t TermLevel = Structur::size, typename enable = void>
class SimpleTree;

template<typename ... Ts, size_t ... Sizes, size_t Level, size_t TermLevel>
class SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, Level,
	TermLevel, typename enable_if<Level!=TermLevel && Level!=0>::type>
{
public:
	enum : size_t {
		level  = Level,
		size   = get_c<level, Sizes...>::type::value,
	};

	typedef SimpleTree<
		TreeStructure<Extend<Ts, Sizes>...>,
		level+1, TermLevel, void>                     child;
	typedef typename get<level-1, pack<Ts...>>::type value_type;

	constexpr SimpleTree() :
		_value(), _child()
	{}

	template<typename ... Args>
	constexpr SimpleTree(Args&& ... args) :
		_value(arg<level-1>(args...)),
		_child(std::forward<Args>(args)...)
	{}

	constexpr child const& operator[] (unsigned ii)
	{
		return _child[ii];
	}

	value_type&       value()       { return _value; }
	value_type const& value() const { return _value; }

	Array<child, size>& childs() { return _child; }

private:
	value_type         _value;
	Array<child, size> _child;
};


// root
template<typename ... Ts, size_t ... Sizes, size_t TermLevel>
class SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, 0, TermLevel, void>
{
public:
	enum : size_t {
		level  = 0,
		size   = get_c<level, Sizes...>::type::value,
	};

	typedef SimpleTree<
		TreeStructure<Extend<Ts, Sizes>...>,
		level+1, TermLevel>                   child;
	typedef SimpleTree<
		TreeStructure<Extend<Ts, Sizes>...>,
		0, TermLevel, void>                   self;

	constexpr SimpleTree() :
		_child()
	{}

	template<typename ... Args>
	constexpr SimpleTree(Args&& ... args) :
		_child(std::forward<Args>(args)...)
	{}

	constexpr child const& operator[] (unsigned ii)
	{
		return _child[ii];
	}

	Array<child, size>& childs() { return _child; }

private:
	Array<child, size> _child;
};

// leaf
template<typename ... Ts, size_t ... Sizes, size_t TermLevel>
class SimpleTree<TreeStructure<Extend<Ts, Sizes>...>, TermLevel, TermLevel, void>
{
public:
	enum : size_t {
		level = TermLevel,
	};

	typedef typename get<level-1, pack<Ts...>>::type value_type;

	constexpr SimpleTree() :
		_value()
	{}

	template<typename ... Args>
	constexpr SimpleTree(Args&& ... args) :
		_value(arg<level-1>(args...))
	{}

	value_type&       value()       { return _value; }
	value_type const& value() const { return _value; }

private:
	value_type _value;

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
