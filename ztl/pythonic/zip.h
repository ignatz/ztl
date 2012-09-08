#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <type_traits>
#include <tuple>

#include <boost/iterator/iterator_facade.hpp>

#include "ztl/runtime.h"

namespace ZTL {

template<typename ... Containers>
struct zip_iter;

template<typename ... Containers>
struct zip_iter
{
	typedef std::tuple<
			std::reference_wrapper<typename Containers::value_type>...
		> tuple_type;

	void operator++ ()
	{
		next(mIts, typename range<2>::type {});
	}

	tuple_type operator* ()
	{
		return deref(mIts, typename range<2>::type {});
	}

	bool operator!= (zip_iter<Containers...> const& e) const
	{
		// TODO: maybe take end from shortest iteratable
		return (std::get<0>(mIts)) != (std::get<0>(e.mIts));
	}

	std::tuple<typename Containers::iterator ...> mIts;
private:

	template<template<typename ...> class Range, typename ... Args,
		typename ... Ns>
	std::tuple<std::reference_wrapper<typename Args::value_type>...>
	deref(
		std::tuple<Args...> const& t,
		Range<Ns...>)
	{
		return std::make_tuple(std::ref(*std::get<Ns::value>(t))...);
	}

	template<template<typename ...> class Range,
		typename ... Args, typename ... Ns>
	void next(
		std::tuple<Args...>& t,
		Range<Ns...>)
	{
		unpack_fun(++std::get<Ns::value>(t)...);
	}

};


template<typename ... Containers>
struct zip_proxy
{
	zip_proxy(Containers& ... its) :
		mBegins(make_tuple(std::begin(its) ...))
		//mEnds(make_tuple(end(its)...))
	{}

	zip_iter<Containers...> begin()
	{
		return {mBegins};
	};

	zip_iter<Containers...> end()
	{
		// TODO: must be ends
		return {mBegins};
	};

	typedef std::tuple<
			typename Containers::iterator ...
		> tuple_type;

	tuple_type mBegins;
	//tuple_type mEnds;
};

template<typename ... Containers>
zip_proxy<Containers ...> zip(Containers& ... its)
{
	return zip_proxy<Containers...> (its...);
}

} // namespace ZTL
