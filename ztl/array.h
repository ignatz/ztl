#pragma once

// Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <cstdint>

namespace ZTL {

using std::size_t;

} // namespace ZTL

#include "ztl/array/impl.h"

#ifndef __WITHOUT_BOOST__
#include "ztl/array/serialize.h"
#endif // __WITHOUT_BOOST__
