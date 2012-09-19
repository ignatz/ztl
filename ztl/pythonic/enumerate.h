#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>
#include <tuple>

#include <boost/iterator/iterator_facade.hpp>

#include "ztl/runtime.h"

namespace ZTL {

template<typename Container>
using enum_wrapped_value = std::pair<size_t,
	typename Container::value_type&>;

template<typename Container>
struct enum_pair_iter :
	public boost::iterator_facade<
		enum_pair_iter<Container>,
		enum_wrapped_value<Container>,
		boost::incrementable_traversal_tag,
		enum_wrapped_value<Container>
	>
{
public:
	enum_pair_iter()
		: _cnt(0), _it()
	{}

	enum_pair_iter(typename Container::iterator it)
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

	enum_wrapped_value<Container>
	dereference() const
	{
		return make_pair(_cnt, std::ref(*_it));
	}

	// members
	size_t _cnt;
	typename Container::iterator _it;
};


template<typename Container>
struct enumerate_proxy
{
public:
	typedef enum_wrapped_value<Container>   value_type;
	typedef typename Container::size_type   size_type;
	typedef enum_pair_iter<Container>       iterator;
	typedef enum_pair_iter<Container const> const_iterator;

	enumerate_proxy(Container& it) :
		_ref(it)
	{}

	typename Container::size_type size() const
	{
		return _ref.size();
	}

	iterator begin()
	{
		return _ref.begin();
	};

	iterator end()
	{
		return _ref.end();
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
