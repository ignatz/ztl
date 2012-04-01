Zero Template Library
=====================

The 0TL Library provides hybrid template functionality on top of C++11's
beautiful standard template library.

We known from experience that often there can be a huge difference between an
elegant and a good solution. 0TL clearly aims for an elegant one. If you are
looking for a good one look at boost fusion. The basic philosophy behind 0TL is
_zero-compromises_, which means zero compiler compatibility trade-offs
affecting code redundancy or readability. In turn, this also means zero
portability guarantees. At the moment, the library breaks even the most recent
compilers. Therefore, it is recommended to use the latest g++-4.7.

What does hybrid mean?
----------------------
Pure template meta programming is limited to computation on types, while C++'s
core language normally operates on instances of types. Making the transition
from the compile time to the runtime domain is usually tricky. Fortunately,
C++11 also provides new language features for compile time static instances,
namely `constexpr`. This allows for flexible templated containers, well suited
for runtime computation. A nice example of such an modern hybrid template
container is `std::tuple`.

Features
--------
* array classes able to invoke non-default constructors similar to
  `std::vector`'s emplace
* compile time tree data structures
* framework for variadic argument pack manipulation
* support for boost serialization
* ...

Licensing
---------
Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)

Distributed under the terms of the GPLv2 or newer
