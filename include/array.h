#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>
#include <cassert>
#include <array>
#include <type_traits>
#include <initializer_list>
#include <boost/serialization/serialization.hpp>

namespace ZTL {

	template<typename NoArrayType, size_t N>
		class ArrayInterface;



	template<typename T>
		struct is_array
		{
			enum : bool { value = std::is_array<T>::value };
		};

	template<typename T, size_t N>
		struct is_array<std::array<T, N>>
		{
			enum : bool { value = true };
		};

	template<typename T, size_t N>
		struct is_array<ArrayInterface<T, N>>
		{
			enum : bool { value = true };
		};



	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		class ArrayInterface<ArrayType<T, N, 0>, N>
		{
			public:
				typedef ArrayType<T, N, 0>  array_type;
				typedef T                   type;
				typedef T                   value_type;
				typedef T*                  iterator;
				typedef T const*            const_iterator;
				typedef T&                  reference;
				typedef T const&            const_reference;

				enum : size_t { size = N };

				template<typename ... Args>
					constexpr ArrayInterface(Args&& ... args) noexcept(std::is_nothrow_constructible<T>::value) :
						array(std::forward<Args>(args)...) {}

				constexpr ArrayInterface(std::initializer_list<type> const& il) : array(il) {}

				inline reference operator[] (size_t const ii) {
					assert( ii < N && "out of range" );
					return *(&array.value+ii);
				}

				inline const_reference operator[] (size_t const ii) const {
					assert( ii < N && "out of range" );
					return *(&array.value+ii);
				}

				// iterators
				iterator                  begin() { return &array.value; }
				constexpr const_iterator  begin() { return &array.value; }
				constexpr const_iterator cbegin() { return begin(); }

				iterator                  end()   { return &array.value+N; }
				constexpr const_iterator  end()   { return &array.value+N; }
				constexpr const_iterator cend()   { return end(); }

				constexpr operator T* ()          { return cbegin(); }

				array_type array;
		};

	// comparisons
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator== (const ArrayInterface<ArrayType<T, N, 0>, N>& x,
						 const ArrayInterface<ArrayType<T, N, 0>, N>& y) {
			return std::equal(x.begin(), x.end(), y.begin());
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator< (const ArrayInterface<ArrayType<T, N, 0>, N>& x,
						const ArrayInterface<ArrayType<T, N, 0>, N>& y) {
			return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator!= (const ArrayInterface<ArrayType<T, N, 0>, N>& x,
						 const ArrayInterface<ArrayType<T, N, 0>, N>& y) {
			return !(x==y);
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator> (const ArrayInterface<ArrayType<T, N, 0>, N>& x,
						const ArrayInterface<ArrayType<T, N, 0>, N>& y) {
			return y<x;
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator<= (const ArrayInterface<ArrayType<T, N, 0>, N>& x,
						 const ArrayInterface<ArrayType<T, N, 0>, N>& y) {
			return !(y<x);
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator>= (const ArrayInterface<ArrayType<T, N, 0>, N>& x,
						 const ArrayInterface<ArrayType<T, N, 0>, N>& y) {
			return !(x<y);
		}



	template<typename T, size_t N, size_t Idx = 0>
		struct StandardArray
		{
			T value;

			// those have to work for any array
			constexpr StandardArray() : value(), next() {}
			constexpr StandardArray(std::initializer_list<T> const& l) :
				value(Idx >= l.size() ? T() : *(l.begin()+Idx)), next(l) {}


			// initialize with [] containers
			template<template <typename, size_t> class Container, typename Type>
				constexpr StandardArray(Container<Type, N> const & con) : value(con[Idx]), next(con) {}

			// invoke non-default constructors of T
			template<typename Arg0, typename ... Args, typename = typename std::enable_if<
				!ZTL::is_array<typename std::decay<Arg0>::type>::value>::type>
				constexpr StandardArray(Arg0&& arg0, Args&& ... args) :
					value(std::forward<Arg0>(arg0), std::forward<Args>(args)...),
					next(std::forward<Arg0>(arg0), std::forward<Args>(args)...) {}

			StandardArray<T, N, Idx+1> next;
		};

	template<typename T, size_t N>
		struct StandardArray<T, N, N>
		{
			template<typename ... Args> constexpr StandardArray(Args&& ...) {}
		};



	template<typename T, size_t N, size_t Idx = 0>
		struct RecursiveArray
		{
			T value;

			constexpr RecursiveArray() : value(), next() {}
			constexpr RecursiveArray(RecursiveArray const& ar) : value(ar.value), next(ar.next) {}
			constexpr RecursiveArray(std::initializer_list<T> const& l) :
				value(Idx >= l.size() ? T() : *(l.begin()+Idx)), next(l) {}

			// initial constructor (unpack array interface)
			template<template<typename, size_t, size_t> class ArrayType,
					size_t Size, size_t CurIndex, typename ... Args,
					typename = typename std::enable_if<(Size<=N && CurIndex==0)>::type>
				constexpr RecursiveArray(ArrayInterface<ArrayType<T, Size, CurIndex>, Size> const& ai, Args&& ... args) :
					RecursiveArray(ai.array, std::forward<Args>(args)...) {}

			template<template<typename, size_t, size_t> class ArrayType, size_t Size, typename ... Args,
					typename = typename std::enable_if<(Size>Idx)>::type>
				constexpr RecursiveArray(ArrayType<T, Size, Idx> const& ar, Args&& ... args) :
					value(ar.value), next(ar.next, std::forward<Args>(args)...) {}

			template<template<typename, size_t, size_t> class ArrayType, size_t Size,
					typename Arg0, typename ... Args>
				constexpr RecursiveArray(ArrayType<T, Size, Size> const& ar, Arg0&& arg0, Args&& ... args) :
					value(arg0), next(ar, std::forward<Args>(args)...) {}

			template<template<typename, size_t, size_t> class ArrayType, size_t Size>
				constexpr RecursiveArray(ArrayType<T, Size, Size> const&) : RecursiveArray() {}

			RecursiveArray<T, N, Idx+1> next;
		};

	template<typename T, size_t N>
		struct RecursiveArray<T, N, N>
		{
			template<typename ... Args> constexpr RecursiveArray(Args&& ...) {}
		};



	template<typename T, size_t N, size_t Idx = 0>
		struct EnumArray
		{
			T value;

			constexpr EnumArray() : value(Idx), next() {}
			constexpr EnumArray(std::initializer_list<T> const& l) :
				value(Idx >= l.size() ? EnumArray() : T(Idx, *(l.begin()+Idx))), next(l) {}

			template<typename ... Args>
				constexpr EnumArray(Args&& ... args) :
					value(Idx, std::forward<Args>(args)...),
					next(std::forward<Args>(args)...) {}

			EnumArray<T, N, Idx+1> next;
		};

	template<typename T, size_t N>
		struct EnumArray<T, N, N>
		{
			template<typename ... Args>
				constexpr EnumArray(Args&& ...) {}
		};



	template<typename T, size_t N>
		using Array = ArrayInterface<StandardArray<T, N>, N>;

	template<typename T, size_t N>
		using Enum = ArrayInterface<EnumArray<T, N>, N>;

	template<typename T, size_t N>
		using ArrayA = ArrayInterface<RecursiveArray<T, N>, N>;

} // ZTL

namespace boost {
	namespace serialization {

		template<typename Archiver, template<typename, size_t, size_t> class Class, typename T, size_t N>
			void serialize(Archiver & ar, ZTL::ArrayInterface<Class<T, N, 0>, N> & s, unsigned int const) {
				ar & s.array;
			}

		// TODO: unsafe, possible collisions with non-ArrayType types with same signature -> use macro action
		template<typename Archiver, template<typename, size_t, size_t> class Class,
				typename T, size_t N, size_t Idx>
			typename std::enable_if<(N>Idx), void>::type
			serialize(Archiver & ar, Class<T, N, Idx> & s, unsigned int const) {
				ar & s.value;
				ar & s.next;
			}

		template<typename Archiver, template<typename, size_t, size_t> class Class,
				typename T, size_t N, size_t Idx>
			typename std::enable_if<(N==Idx), void>::type
			serialize(Archiver &, Class<T, N, Idx> &, unsigned int const) {}
	} // namespace serialization
} // namespace boost
