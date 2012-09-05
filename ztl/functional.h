#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <functional>
#include <type_traits>
#include <tuple>

#include "pack.h"

namespace ZTL {

template <typename Object, typename ReturnType, typename... Args>
inline std::function<ReturnType(Args...)> bind_mf(ReturnType(Object::*mem_ptr)(Args ...), Object& obj)
{
	return [&,mem_ptr](Args&& ... args) -> ReturnType {
		return (obj.*mem_ptr)(std::forward<Args>(args)...);
	};
}



template<typename Iteratable>
struct Eiterator
{
	typedef size_t size_type;
	typedef typename Iteratable::value_type value_type;
	typedef typename Iteratable::iterator   iterator;

	typedef std::pair<size_type, value_type> pair_type;

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

	bool operator!= (Eiterator<Iteratable> const& e)
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



template<typename Out, typename In, size_t Idx = 0>
struct Transform
{
	template<typename ... Args>
	static Out doit(std::tuple<Args...> const& t, std::function<Out (In)> f)
	{
		return f(std::get<Idx>(t));
	}
};



template<template<typename ...> class Range, typename ... In, typename ... Idx>
std::tuple<In ...> transform(
	std::tuple<In...> const& t,
	Range<Idx...> r = typename range<size<In...>::value>::type {})
{
	std::make_tuple(Transform<int, int, Idx::value>::doit(t, [](int i) { return i+1; })...);
}

//template<typename ... Iteratables>
//struct Ziterator
//{
	//typedef std::tuple<
			//std::reference_wrapper<Iteratables>...
		//> tuple_type;

	//void operator++ ()
	//{
	//}

	//tuple_type operator* ()
	//{
	//}

	//bool operator!= (Ziterator<Iteratable> const& e)
	//{
		//return mIts[0] != e.mIts[0];
	//}

	//std::tuple<typename Iteratable::iterator ...> mIts;
//};


//template<typename ... Iteratables>
//struct Zip
//{
	//Zip(Iteratables& ... its) :
		//mBegins(make_tuple(begin(its)...)),
		//mEnds(make_tuple(end(its)...))
	//{}

	//Ziterator<Iteratables...> begin()
	//{
		//return Ziterator<Iteratables...> {mBegins};
	//};

	//Ziterator<Iteratables...> end()
	//{
		//return Ziterator<Iteratables...> {mEnds};
	//};

	//typedef std::tuple<
			//typename Iteratables::iterator ...
		//> tuple_type;

	//tuple_type mBegins;
	//tuple_type mEnds;
//};

//template<typename ... Iteratables>
//Zip<Iteratables ...> zip(Iteratables& ... its)
//{
	//return Zip<Iteratables...> (its...);
//}

} // namespace ZTL
