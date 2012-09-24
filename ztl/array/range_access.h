#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

// missing free functions for cbegin() and cend()
template<typename T>
inline auto cbegin(T& t) -> decltype(t.cbegin())
{
	return t.cbegin();
}

template<typename T>
inline auto cend(T& t) -> decltype(t.cend())
{
	return t.cend();
}

template<typename T, size_t N>
inline typename std::remove_const<T>::type const* cbegin(T(& t)[N])
{
	return t;
}

template<typename T, size_t N>
inline typename std::remove_const<T>::type const* cend(T(& t)[N])
{
	return t + N;
}



// missing free functions for rbegin() and rend()
template<typename T>
inline auto rbegin(T& t) ->decltype(t.rbegin())
{
	return t.rbegin();
}

template<typename T>
inline auto rend(T& t) ->decltype(t.rend())
{
	return t.rend();
}

template<typename T, size_t N>
typename std::reverse_iterator<T*> rbegin(T(& t)[N])
{
	return std::reverse_iterator<T*>(t + N);
}

template<typename T, size_t N>
typename std::reverse_iterator<T*> rend(T(& t)[N])
{
	return std::reverse_iterator<T*>(t);
}



// missing free functions for crbegin() and crend()
template<typename T>
inline auto crbegin(T& t) ->decltype(t.crbegin())
{
	return t.crbegin();
}

template<typename T>
inline auto crend(T& t) ->decltype(t.crend())
{
	return t.crend();
}

template<typename T, size_t N>
typename std::reverse_iterator<T const*> crbegin(T(& t)[N])
{
	return std::reverse_iterator<typename std::remove_const<T>::type const*>(t + N);
}

template<typename T, size_t N>
typename std::reverse_iterator<T const*> crend(T(& t)[N])
{
	return std::reverse_iterator<typename std::remove_const<T>::type const*>(t);
}
