#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdlib>
#include <cassert>
#include <type_traits>
#include <boost/serialization/serialization.hpp>

namespace ZTL {

	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		class ArrayInterface
		{
			public:
				typedef T                 type;
				typedef T                 value_type;
				typedef T*                iterator;
				typedef T const*          const_iterator;
				typedef T&                reference;
				typedef T const&          const_reference;

				enum : size_t { size = N };

				template<typename Type, size_t Size, typename ... Args>
					constexpr ArrayInterface(ArrayInterface<ArrayType, Type, Size> const& ar, Args ... args) :
						array(ar.array, args...) {
							static_assert((sizeof...(Args) + Size) == size, "wrong number of arguments");
						}

				template<typename ... Args>
					constexpr ArrayInterface(Args ... args) : array(args...) {}

				constexpr ArrayInterface() : array() {}

				// compiler killer - WTF
				//constexpr ArrayInterface(T const& t) : array(t) {}

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

			//private:
				ArrayType<value_type, size, 0> array;

				// TODO: make non-intrusive as soon as compiler stops fucking around
				friend class boost::serialization::access;
				template<typename Archiver>
					void serialize(Archiver& ar, unsigned int const version) {
						static_cast<void>(version);
						ar & array;
					}
		};

	// comparisons
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator== (const ArrayInterface<ArrayType, T, N>& x,
						 const ArrayInterface<ArrayType, T, N>& y) {
			return std::equal(x.begin(), x.end(), y.begin());
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator< (const ArrayInterface<ArrayType, T, N>& x,
						const ArrayInterface<ArrayType, T, N>& y) {
			return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator!= (const ArrayInterface<ArrayType, T, N>& x,
						 const ArrayInterface<ArrayType, T, N>& y) {
			return !(x==y);
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator> (const ArrayInterface<ArrayType, T, N>& x,
						const ArrayInterface<ArrayType, T, N>& y) {
			return y<x;
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator<= (const ArrayInterface<ArrayType, T, N>& x,
						 const ArrayInterface<ArrayType, T, N>& y) {
			return !(y<x);
		}
	template<template<typename, size_t, size_t> class ArrayType, typename T, size_t N>
		bool operator>= (const ArrayInterface<ArrayType, T, N>& x,
						 const ArrayInterface<ArrayType, T, N>& y) {
			return !(x<y);
		}


	template<typename T, size_t N, size_t Idx = 0>
		struct EnumData;

	template<typename T, size_t N, size_t Idx = 0>
		struct ArrayData
		{
			static_assert(N>Idx, "this should not happen. Use the interface.");

			T value;

			// appending constructor
			template<typename Type, size_t Size, size_t Index, typename = typename std::enable_if<(Size>Index)>::type, typename ... Args>
				constexpr ArrayData(ArrayData<Type, Size, Index> const& ar, Args ... args) :
					value(ar.value),
					next(ar.next, args ...) {}

			template<typename Type, size_t Size, size_t Index, typename Arg, typename = typename std::enable_if<(Size==Index)>::type, typename ... Args>
				constexpr ArrayData(ArrayData<Type, Size, Index> const& ar, Arg a, Args ... args) :
					value(a),
					next(ar, args ...) {}


			// initialize with [] containers
			template<template <typename, size_t> class Container, typename Type>
				constexpr ArrayData(Container<Type, N> const & con) : value(con[Idx]), next(con) {}

			template<typename Type>
				constexpr ArrayData(ArrayInterface<EnumData, Type, N> const & con) : value(con[Idx]), next(con) {}
			template<typename Type>
				constexpr ArrayData(ArrayInterface<ArrayData, Type, N> const & con) : value(con[Idx]), next(con) {}


			// generic emblacement comstructor; NOTE: defined last due to template evaluation order
			template<typename ... Args>
				constexpr ArrayData(Args ... args) : value(args ...), next(args ...) {}

			ArrayData<T, N, Idx+1> next;
		};

	template<typename T, size_t N>
		struct ArrayData<T, N, N>
		{
			template<typename ... Args> constexpr ArrayData(Args ...) {}
		};



	template<typename T, size_t N, size_t Idx>
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
		using Array = ArrayInterface<ArrayData, T, N>;

	template<typename T, size_t N>
		using Enum = ArrayInterface<EnumData, T, N>;

} // ZTL

namespace boost {
	namespace serialization {
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
