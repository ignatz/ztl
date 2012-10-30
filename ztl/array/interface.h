#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdint>
#include <cassert>
#include <type_traits>
#include <initializer_list>
#include <iterator>

#include "ztl/array/traits.h"
#include "ztl/type_traits.h"

namespace ZTL {

using std::size_t;

template<template<typename, size_t, size_t, typename> class ArrayType, typename T, size_t N>
class ArrayInterface<ArrayType<T, N, 0, void>, N>
{
public:
	typedef ArrayType<T, N, 0, void>               array_type;
	typedef ArrayInterface<array_type, N>          self_type;
	typedef T                                      value_type;
	typedef T*                                     pointer;
	typedef T const*                               const_pointer;
	typedef value_type&                            reference;
	typedef value_type const&                      const_reference;
	typedef value_type*                            iterator;
	typedef value_type const*                      const_iterator;
	typedef std::size_t                            size_type;
	typedef std::ptrdiff_t                         difference_type;
	typedef std::reverse_iterator<iterator>        reverse_iterator;
	typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;

	constexpr ArrayInterface() : array() {}
	constexpr ArrayInterface(self_type const& c) : array(c.array) {}

	template<typename Arg0, typename ... Args, typename = typename
		enable_if<!std::is_same<self_type, typename std::decay<Arg0>::type>::value>::type>
	constexpr ArrayInterface(Arg0&& arg0, Args&& ... args) \
			noexcept(std::is_nothrow_constructible<T>::value) :
		array(std::forward<Arg0>(arg0), std::forward<Args>(args)...) {}

	constexpr ArrayInterface(std::initializer_list<value_type> const& il) noexcept :
		array(il) {}

	inline reference operator[] (size_t const ii) {
		assert( ii < N && "out of range" );
		return *(&array.value+ii);
	}

	inline const_reference operator[] (size_t const ii) const {
		assert( ii < N && "out of range" );
		return *(&array.value+ii);
	}

	// iterators
	iterator                    begin()       { return &array.value; }
	constexpr const_iterator    begin() const { return &array.value; }
	constexpr const_iterator   cbegin() const { return begin(); }
	reverse_iterator             rend() { return reverse_iterator(begin()); }
	constexpr const_reverse_iterator crend() const {
		return const_reverse_iterator(cbegin());
	}

	iterator                    end()       { return &array.value+N; }
	constexpr const_iterator    end() const { return &array.value+N; }
	constexpr const_iterator   cend() const { return end(); }
	reverse_iterator         rbegin() { return reverse_iterator(end()); }
	constexpr const_reverse_iterator crbegin() const {
		return const_reverse_iterator(cend());
	}

	// casting
	template<typename Type, typename = typename enable_if<
			ZTL::is_array<Type>::value>::type>
	operator Type const& () const
	{
		return reinterpret_cast<Type const&>(*this);
	}

	template<typename Type, typename = typename enable_if<
			std::is_array<Type>::value>::type>
	operator Type& ()
	{
		return reinterpret_cast<Type&>(*this);
	}

	template<template<typename, size_t> class Array, typename Type,
		typename = typename std::enable_if<ZTL::is_array<Array<Type, N>>::value>::type>
	operator Array<Type, N>& ()
	{
		return reinterpret_cast<Array<Type, N>&>(*this);
	}


	// stl conformity
	static constexpr size_type size() { return N; }

	void fill(value_type const& u) { std::fill_n(begin(), size(), u);}

	template<typename Type,
		typename = typename enable_if<is_array<Type>::value>::type>
	void swap(Type& other)
	{
		std::swap_ranges(begin(), end(), begin(other));
	}

	T*       data()       { return begin(); }
	T const* data() const { return cbegin(); }

private:
	array_type array;
};

// comparisons
template<template<typename, size_t, size_t, typename...> class ArrayType,
	typename T, size_t N, typename ... Args>
bool operator== (const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& x,
	const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& y)
{
	return std::equal(x.begin(), x.end(), y.begin());
}

template<template<typename, size_t, size_t, typename...> class ArrayType,
	typename T, size_t N, typename ... Args>
bool operator< (const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& x,
	const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& y)
{
	return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}

template<template<typename, size_t, size_t, typename...> class ArrayType,
	typename T, size_t N, typename ... Args>
bool operator!= (const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& x,
	const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& y)
{
	return !(x==y);
}

template<template<typename, size_t, size_t, typename...> class ArrayType,
	typename T, size_t N, typename ... Args>
bool operator> (const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& x,
	const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& y)
{
	return y<x;
}

template<template<typename, size_t, size_t, typename...> class ArrayType,
	typename T, size_t N, typename ... Args>
bool operator<= (const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& x,
	const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& y)
{
	return !(y<x);
}

template<template<typename, size_t, size_t, typename...> class ArrayType,
	typename T, size_t N, typename ... Args>
bool operator>= (const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& x,
	const ArrayInterface<ArrayType<T, N, 0, Args ...>, N>& y)
{
	return !(x<y);
}

} // ZTL
