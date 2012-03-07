Zero Template Library
=====================

The 0TL Library provides hybrid template functionality on top of C++11's
beautiful standard template library and little boost.

Our experience thought us that often there is a huge difference between an
elegant and a good solution. 0TL clearly aims for an elegant one. If you are
looking for a good one look at boost::fusion. The main philosophy behind 0TL is
_zero-compromises_, which means no trade-offs concerning compiler
compatibilities affecting code redundancy or readability. In turn, this also
means no portability guarantees. At the moment, the code breaks even the most
recent compilers. Therefore, it is recommended to use current g++-4.7 snapshots.

What does hybrid mean?
----------------------
Pure template meta programming is limited to computation on types, while C++'s
core language normally operates on instances of types. Making the transition
from the compile time to the runtime domain is usually tricky. Fortunately,
C++11 also provides new language features for compile time static instances,
namely `constexpr`. This allows for flexible templated containers, well suited
for runtime computation. A good examples of an modern hybrid template container
is std::tuple.

Features
--------
* array class, which can invoke non-default constructors
* static tree structure
* computation on variadic argument packs

Licensing
---------
Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)

Distributed under the terms of the GPLv2 or newer
