#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>
#include <tuple>
#include <functional>

#include <boost/iterator/iterator_facade.hpp>

#include "ztl/pack.h"

namespace ZTL {

template<typename Container>
struct enum_pair_traits
{
	typedef typename std::conditional<
		std::is_const<Container>::value,
		typename Container::const_reference,
		typename Container::reference
	>::type reference;

	typedef std::pair<size_t, reference> type;
};


template<typename Container>
struct enum_pair_iter :
	public boost::iterator_facade<
		enum_pair_iter<Container>,
		typename enum_pair_traits<Container>::type,
		boost::incrementable_traversal_tag,
		typename enum_pair_traits<Container>::type
	>
{
private:
	typedef typename std::conditional<
		std::is_const<Container>::value,
		typename Container::const_iterator,
		typename Container::iterator
	>::type iterator;

public:
	enum_pair_iter()
		: _cnt(0), _it()
	{}

	enum_pair_iter(iterator it)
		: _cnt(0), _it(it)
	{}

private:
	friend class boost::iterator_core_access;
	template <typename>
	friend class enum_pair_iter;

	template<typename T>
	bool equal(enum_pair_iter<T> const& other) const
	{
		return this->_it == other._it;
	}

	void increment()
	{
		++_it;
		++_cnt;
	}

	typename enum_pair_traits<Container>::type
	dereference() const
	{
		return make_pair(_cnt, std::ref(*_it));
	}

	// members
	size_t _cnt;
	iterator _it;
};


template<typename Container>
struct enumerate_proxy
{
public:
	enumerate_proxy(Container& it) :
		_ref(it)
	{}

	typename Container::size_type size() const
	{
		return _ref.size();
	}

	enum_pair_iter<Container> begin()
	{
		return enum_pair_iter<Container>(_ref.begin());
	};

	enum_pair_iter<Container> end()
	{
		return enum_pair_iter<Container>(_ref.end());
	};

private:
	Container& _ref;
};


template<typename Container>
enumerate_proxy<Container> enumerate(Container& a)
{
	return enumerate_proxy<Container> (a);
}

} // namespace ZTL
