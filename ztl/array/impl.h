#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdint>
#include <cassert>
#include <type_traits>
#include <initializer_list>
#include <iterator>

#include "ztl/array/base.h"
#include "ztl/array/trait.h"
#include "ztl/enable_if.h"

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

	constexpr operator T* () const { return cbegin(); }

	// stl algorithm
	static constexpr size_type size() { return N; }

	void fill(value_type const& u) { std::fill_n(begin(), size(), u);}

	template<typename Type,
		typename = typename enable_if<ZTL::is_array<Type>::value>::type>
	void swap(Type& other) {
		std::swap_ranges(begin(), end(), begin(other));
	}

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



template<typename T, size_t N, size_t Idx>
struct StandardArray<T, N, Idx, typename enable_if<(Idx+1<N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr StandardArray() : BaseArray<T, N, Idx>() {}

	// invoke non-default constructors of T
	template<typename ... Args>
	constexpr StandardArray(Args&& ... args) :
		BaseArray<T, N, Idx>(args...), next(std::forward<Args>(args)...) {}

	StandardArray<T, N, Idx+1> next;
};

template<typename T, size_t N, size_t Idx>
struct StandardArray<T, N, Idx, typename enable_if<(Idx+1>=N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr StandardArray() : BaseArray<T, N, Idx>() {}

	// invoke non-default constructors of T
	template<typename ... Args>
		constexpr StandardArray(Args&& ... args) :
			BaseArray<T, N, Idx>(std::forward<Args>(args)...) {}
};



template<typename T, size_t N, size_t Idx>
struct RecursiveArray<T, N, Idx, typename enable_if<(Idx+1<N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr RecursiveArray() : BaseArray<T, N, Idx>(), next() {}
	constexpr RecursiveArray(std::initializer_list<T> const& l) :
		BaseArray<T, N, Idx>(l), next(l) {}


	template<template<typename, size_t> class Container, typename Q, size_t Size, typename ... Args,
		typename = typename enable_if<(Idx<Size)>::type>
	constexpr RecursiveArray(Container<Q, Size> const& con, Args&& ... args) :
		BaseArray<T, N, Idx>(*(con.begin()+Idx)), next(con, std::forward<Args>(args)...)
	{
		static_assert(Size + sizeof...(Args) <= N, "too many arguments");
	}

	template<template<typename, size_t> class Container, typename Q, size_t Size,
		typename = typename enable_if<(Idx>=Size)>::type>
	constexpr RecursiveArray(Container<Q, Size> const&) : RecursiveArray() {}

	template<template<typename, size_t> class Container, typename Q, size_t Size, typename Arg0,
		typename ... Args, typename = typename enable_if<(Idx>=Size)>::type>
	constexpr RecursiveArray(Container<Q, Size> const& con, Arg0&& arg0, Args&& ... args) :
		BaseArray<T, N, Idx>(std::forward<Arg0>(arg0)), next(con, std::forward<Args>(args)...) {}

	RecursiveArray<T, N, Idx+1> next;
};

template<typename T, size_t N, size_t Idx>
struct RecursiveArray<T, N, Idx, typename enable_if<(Idx+1>=N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr RecursiveArray() : BaseArray<T, N, Idx>() {}
	constexpr RecursiveArray(std::initializer_list<T> const& l) :
		BaseArray<T, N, Idx>(l) {}


	template<template<typename, size_t> class Container, typename Type, size_t Size, typename ... Args,
		typename = typename enable_if<(Idx<Size)>::type>
	constexpr RecursiveArray(Container<Type, Size> const& con) :
		BaseArray<T, N, Idx>(*(con.begin()+Idx)) {}

	template<template<typename, size_t> class Container, typename Type, size_t Size,
		typename ... Args, typename = typename enable_if<(Idx>=Size)>::type>
	constexpr RecursiveArray(Container<Type, Size> const&, Args&& ... args) :
		BaseArray<T, N, Idx>(std::forward<Args>(args)...)
	{
		static_assert(sizeof...(Args) <= 1, "too many arguments");
	}
};



template<typename T, size_t N, size_t Idx>
struct EnumArray<T, N, Idx, typename enable_if<(Idx+1<N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr EnumArray() : BaseArray<T, N, Idx>(Idx), next() {}

	template<typename ... Args>
	constexpr EnumArray(Args&& ... args) :
		BaseArray<T, N, Idx>(Idx, args...), next(std::forward<Args>(args)...) {}

	EnumArray<T, N, Idx+1> next;
};

template<typename T, size_t N, size_t Idx>
struct EnumArray<T, N, Idx, typename enable_if<(Idx+1>=N), void>::type> :
	public BaseArray<T, N, Idx>
{
	constexpr EnumArray() : BaseArray<T, N, Idx>(Idx) {}

	template<typename ... Args>
	constexpr EnumArray(Args&& ... args) :
		BaseArray<T, N, Idx>(Idx, args...){}
};



template<typename T, size_t N>
using Array = ArrayInterface<StandardArray<T, N>, N>;

template<typename T, size_t N>
using Enum = ArrayInterface<EnumArray<T, N>, N>;

template<typename T, size_t N>
using ArrayA = ArrayInterface<RecursiveArray<T, N>, N>;

} // ZTL
