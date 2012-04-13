#pragma once

#include <type_traits>
#include <tuple>

#include "pack.h"

// extensions for std::tuple
namespace ZTL {

template<typename Source, typename Target = std::tuple<>>
	struct Flatten;

template<typename Type, typename ... SourceArgs, typename ... TargetArgs>
	struct Flatten<stack<Type, SourceArgs...>, std::tuple<TargetArgs...>>
	{
		typedef Flatten<
			stack<SourceArgs...>,
			typename merge<std::tuple<TargetArgs...>, typename promote<std::tuple, Type>::type>::type
				> next_type;
		typedef typename next_type::type type;

		static type unpack(Type&& cur, SourceArgs&& ... args, std::tuple<TargetArgs...>&& t) {
			return next_type::unpack(
				 std::forward<SourceArgs>(args)...,
				 std::tuple_cat(t, promote<std::tuple, Type>::get(std::forward<Type>(cur))));
		}
	};

template<typename ... TargetArgs>
	struct Flatten<stack<>, std::tuple<TargetArgs...>>
	{
		typedef std::tuple<TargetArgs...> type;

		static type unpack(std::tuple<TargetArgs...>&& t) { return t; }
	};



template<typename T, typename SeqType>
	struct call_helper;

template<typename ... Args, typename T, int ... Seq>
	struct call_helper<std::tuple<Args...>, sequence<T, Seq...>>
	{
		template<typename ReturnType, typename ... FArgs>
			static ReturnType call(std::function<ReturnType (FArgs...)> const& f,
								   std::tuple<Args...>&& t) {
				return f(std::get<Seq>(t)...);
			}
	};



template<typename ReturnType, typename ... FArgs, typename ... Args>
	ReturnType call(std::function<ReturnType (FArgs...)> const& f, Args&& ... args) {
		typedef typename Flatten<stack<Args...>, std::tuple<>>::type type;

		return call_helper<
			type, typename make_sequence<int, 0, std::tuple_size<type>::value>::type
		>::call(std::function<ReturnType (FArgs...)>(f),
				Flatten<stack<Args...>>::unpack(
					std::forward<Args>(args)..., std::tuple<>()));
	}

} // namespace ZTL
