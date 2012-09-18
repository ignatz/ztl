#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>
#include <tuple>

#include <boost/iterator/iterator_facade.hpp>

#include "ztl/runtime.h"

namespace ZTL {

// template aliases somehow don't work in this context
//template<typename ... Containers>
//using zip_wrapped_value = std::tuple<typename Containers::value_type& ...>;

//template<typename ... Containers>
//using zip_wrapped_iter = std::tuple<typename Containers::iterator...>;

template<typename ... Containers>
struct zip_tuple_iter :
	public boost::iterator_facade<
		zip_tuple_iter<Containers...>,
		std::tuple<typename Containers::value_type& ...>,
		boost::forward_traversal_tag,
		std::tuple<typename Containers::value_type& ...>
	>
{
public:
	typedef std::tuple<
		typename Containers::value_type& ...> _value_type;
	typedef std::tuple<
		typename Containers::iterator...>     _iter_type;

	zip_tuple_iter() :
		_its()
	{}

	//zip_tuple_iter(zip_wrapped_iter<Containers...> its)
	zip_tuple_iter(_iter_type its)
		: _its(its)
	{}

private:
	friend class boost::iterator_core_access;
	template <typename...>
	friend class zip_tuple_iter;

	template<typename ... T>
	bool equal(zip_tuple_iter<T...> const& other) const
	{
		return this->_its == other._its;
	}

	void increment()
	{
		_increment(typename range<size<pack<Containers...>>::value>::type {});
	}

	_value_type
	dereference() const
	{
		return _dereference(typename range<size<pack<Containers...>>::value>::type {});
	}

	// helpers
	template<template<typename ...> class Range,
		typename ... Ns>
	_value_type
	_dereference(Range<Ns...>) const
	{
		return _value_type(std::ref(*std::get<Ns::value>(_its))...);
	}

	template<template<typename ...> class Range,
		typename ... Ns>
	void _increment(Range<Ns...>)
	{
		unpack_fun(++std::get<Ns::value>(_its)...);
	}

	// member, holding iterator position
	_iter_type _its;
};


template<typename ... Containers>
struct zip_proxy
{
public:
	zip_proxy(Containers& ... its) :
		_ref(its ...)
	{}

	zip_tuple_iter<Containers...> begin()
	{
		return _begin(typename range<size<pack<Containers...>>::value>::type {});
	};

	zip_tuple_iter<Containers...> end()
	{
		return _end(typename range<size<pack<Containers...>>::value>::type {});
	};

private:
	// helper
	template<template<typename ...> class Range,
		typename ... Ns>
	zip_tuple_iter<Containers...>
	_begin(Range<Ns...>)
	{
		return std::make_tuple(
			std::begin(std::get<Ns::value>(_ref))...);
	}

	template<template<typename ...> class Range,
		typename ... Ns>
	zip_tuple_iter<Containers...>
	_end(Range<Ns...>)
	{
		return std::make_tuple(
			std::end(std::get<Ns::value>(_ref))...);
	}

	typename std::tuple<Containers&...> _ref;
};


template<typename ... Containers>
zip_proxy<Containers ...> zip(Containers& ... its)
{
	return zip_proxy<Containers...> (its...);
}

} // namespace ZTL
