#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <functional>
#include <type_traits>
#include <tuple>

#include "functional.h"
#include "pack.h"


template<typename Iteratable>
struct Eiterator
{
	typedef size_t                          size_type;
	typedef typename Iteratable::value_type value_type;
	typedef typename Iteratable::iterator   iterator;

	typedef std::pair<
			size_type,
			std::reference_wrapper<value_type>
		> pair_type;

	// TODO: check return value should be different
	void operator++ ()
	{
		++mCnt;
		++mIt;
	}

	pair_type operator* ()
	{
		return make_pair(mCnt, std::ref(*mIt));
	}

	bool operator!= (Eiterator<Iteratable> const& e) const
	{
		return mIt != e.mIt;
	}

	size_type  mCnt;
	iterator   mIt;
};

template<typename Iteratable>
struct Enumerate
{
	Enumerate(Iteratable& it) :
		mRef(it) {}

	typename Iteratable::size_type size() const
	{
		return mRef.size();
	}

	Eiterator<Iteratable> begin()
	{
		return Eiterator<Iteratable> {0, mRef.begin()};
	};

	Eiterator<Iteratable> end()
	{
		return Eiterator<Iteratable> {mRef.size(), mRef.end()};
	};

	Iteratable& mRef;
};

template<typename Iteratable>
Enumerate<Iteratable> enumerate(Iteratable& a)
{
	return Enumerate<Iteratable> (a);
}



template<size_t N, typename ... Returns, typename ... Args>
typename get<N, pack<Returns...>>::type
transform_helper(
	std::tuple<Args...> const& t,
	std::tuple<std::function<Returns(Args)>...> const& f)
{
	return std::get<N>(f)(std::get<N>(t));
}


template<template<typename ...> class Range, typename ... Returns,
	typename ... Args, typename ... Ns>
std::tuple<Returns...>
transform(
	std::tuple<Args...> const& t,
	std::tuple<std::function<Returns(Args)>...> const& f,
	Range<Ns...>)
{
	return std::make_tuple(std::get<Ns::value>(f)(std::get<Ns::value>(t))...);
}

template<template<typename ...> class Range, typename ... Args,
	typename ... Ns, typename T, typename U>
auto transform(
	std::tuple<Args...> const& t,
	std::function<T (U)> const& f,
	Range<Ns...>)
	-> decltype(std::make_tuple(f(std::get<Ns::value>(t))...))
{
	return std::make_tuple(f(std::get<Ns::value>(t))...);
}


template<typename ... T>
inline void unpack_fun(T&& ...) {}

template<typename ... Iteratables>
struct Ziterator;

template<typename ... Iteratables>
struct Ziterator
{
	typedef std::tuple<
			std::reference_wrapper<typename Iteratables::value_type>...
		> tuple_type;

	void operator++ ()
	{
		next(mIts, typename range<2>::type {});
	}

	tuple_type operator* ()
	{
		return deref(mIts, typename range<2>::type {});
	}

	bool operator!= (Ziterator<Iteratables...> const& e) const
	{
		// TODO: maybe take end from shortest iteratable
		return (std::get<0>(mIts)) != (std::get<0>(e.mIts));
	}

	std::tuple<typename Iteratables::iterator ...> mIts;
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


template<typename ... Iteratables>
struct Zip
{
	Zip(Iteratables& ... its) :
		mBegins(make_tuple(std::begin(its) ...))
		//mEnds(make_tuple(end(its)...))
	{}

	Ziterator<Iteratables...> begin()
	{
		return Ziterator<Iteratables...> {mBegins};
	};

	Ziterator<Iteratables...> end()
	{
		//return Ziterator<Iteratables...> {mEnds};
		return Ziterator<Iteratables...> {mBegins};
	};

	typedef std::tuple<
			typename Iteratables::iterator ...
		> tuple_type;

	tuple_type mBegins;
	//tuple_type mEnds;
};

template<typename ... Iteratables>
Zip<Iteratables ...> zip(Iteratables& ... its)
{
	return Zip<Iteratables...> (its...);
}
