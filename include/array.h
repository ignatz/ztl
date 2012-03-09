#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>
#include <cassert>
#include <array>
#include <type_traits>
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
					constexpr ArrayInterface(Args&& ... args) : array(std::forward<Args>(args)...) {}

				inline reference operator[] (size_t const ii) {
					assert( ii < N && "out of range" );
					return *(&array.value+ii);
				}

				inline const_reference operator[] (size_t const ii) const {
					assert( ii < N && "out of range" );
					return *(&array.value+ii);
				}

				// iterators
				iterator        begin()       { return &array.value; }
				const_iterator  begin() const { return &array.value; }
				const_iterator cbegin() const { return &array.value; }

				iterator        end()       { return &array.value+N; }
				const_iterator  end() const { return &array.value+N; }
				const_iterator cend() const { return &array.value+N; }

				operator T* () { return &array.value; }

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
		struct ArrayData
		{
			T value;

			constexpr ArrayData() : value(), next() {}

			// initialize with [] containers
			template<template <typename, size_t> class Container, typename Type>
				constexpr ArrayData(Container<Type, N> const & con) : value(con[Idx]), next(con) {}


			template<typename Arg0, typename ... Args, typename = typename std::enable_if<!ZTL::is_array<typename std::decay<Arg0>::type>::value>::type>
				constexpr ArrayData(Arg0&& arg0, Args&& ... args) :
					value(std::forward<Arg0>(arg0), std::forward<Args>(args)...),
					next(std::forward<Arg0>(arg0), std::forward<Args>(args)...) {}

			ArrayData<T, N, Idx+1> next;
		};

	template<typename T, size_t N>
		struct ArrayData<T, N, N>
		{
			template<typename ... Args> constexpr ArrayData(Args&& ...) {}
		};




	template<typename T, size_t N, size_t Idx = 0>
		struct AppendingData
		{
			T value;

			constexpr AppendingData() : value(), next() {}

			// initialize with [] containers
			template<template <typename, size_t> class Container, typename Type>
				constexpr AppendingData(Container<Type, N> const & con) : value(con[Idx]), next(con) {}


			// initial constructor (unpack interface)
			template<template<typename, size_t, size_t> class ArrayType,
					size_t Size, size_t CurIndex, typename ... Args,
					typename = typename std::enable_if<(Size<=N && CurIndex==0 && Size>0)>::type>
				constexpr AppendingData(ArrayInterface<ArrayType<T, Size, CurIndex>, Size> const& ai, Args&& ... args) :
					value(ai.array.value),
					next(ai.array.next, std::forward<Args>(args)...) {}

			template<template<typename, size_t, size_t> class ArrayType, size_t Size, size_t CurIndex,
					typename Arg0, typename ... Args, typename = typename std::enable_if<(CurIndex==Size)>::type>
				constexpr AppendingData(ArrayInterface<ArrayType<T, Size, CurIndex>, Size> const& ai, Arg0&& arg0, Args&& ... args) :
					value(arg0),
					next(ai, args...) {}

			template<template<typename, size_t, size_t> class ArrayType, size_t Size, typename ... Args,
					typename = typename std::enable_if<(Size<Idx)>::type>
				constexpr AppendingData(ArrayType<T, Size, Idx> const& ar, Args&& ... args) :
					value(ar.value), next(ar.next, std::forward<Args>(args)...) {}

			template<template<typename, size_t, size_t> class ArrayType, size_t Size,
					typename Arg0, typename ... Args, typename = typename std::enable_if<Size==Idx>::type>
				constexpr AppendingData(ArrayType<T, Size, Idx> const& ar, Arg0&& arg0, Args&& ... args) :
					value(arg0), next(std::forward<Args>(args)...) {}


			AppendingData<T, N, Idx+1> next;
		};

	template<typename T, size_t N>
		struct AppendingData<T, N, N>
		{
			template<typename ... Args> constexpr AppendingData(Args&& ...) {}
		};



	template<typename T, size_t N, size_t Idx = 0>
		struct EnumData
		{
			T value;

			template<typename ... Args>
				constexpr EnumData(Args ... args) :
					value(Idx, args ...),
					next(args ...) {}

			EnumData<T, N, Idx+1> next;
		};

	template<typename T, size_t N>
		struct EnumData<T, N, N>
		{
			template<typename ... Args>
				constexpr EnumData(Args ...) {}
		};



	template<typename T, size_t N>
		using Array = ArrayInterface<ArrayData<T, N>, N>;

	template<typename T, size_t N>
		using Enum = ArrayInterface<EnumData<T, N>, N>;

	// TODO: stupid name
	template<typename T, size_t N>
		using Appending = ArrayInterface<AppendingData<T, N>, N>;

} // ZTL

namespace boost {
	namespace serialization {

		template<typename Archiver, template<typename, size_t, size_t> class Class, typename T, size_t N>
			void serialize(Archiver & ar, ZTL::ArrayInterface<Class<T, N, 0>, N> & s, unsigned int const) {
				ar & s.array;
			}

		// TODO: unsafe, possible collisions with non-ArrayType types with same signature -> use macro action
		template<typename Archiver, template<typename, size_t, size_t> class Class, typename T, size_t N, size_t Idx>
			typename std::enable_if<(N>Idx), void>::type serialize(Archiver & ar, Class<T, N, Idx> & s, unsigned int const) {
				ar & s.value;
				ar & s.next;
			}

		template<typename Archiver, template<typename, size_t, size_t> class Class, typename T, size_t N, size_t Idx>
			typename std::enable_if<(N==Idx), void>::type serialize(Archiver &, Class<T, N, Idx> &, unsigned int const) {}
	} // namespace serialization
} // namespace boost
