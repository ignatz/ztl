#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

namespace ZTL {

template<typename ... Args>      struct pack {};
template<typename T, T ... Args> struct pack_c {};

} // namespace ZTL
