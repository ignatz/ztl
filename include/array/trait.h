#pragma once

#include <type_traits>

namespace ZTL {

// forward decl
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

} // namespace ZTL
