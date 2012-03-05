Zero Template Library
=====================

The 0TL provides hybrid template functionality on top of C++11's beautiful standard template library and boost.
The philosophy behind 0TL is _zero-compromises_. Which means, no trade-offs concerning compiler compatibilities affecting code redundancy and readability. Right now, the code breaks even many recent compilers.
It is recommended to either try clang3.x or g++-4.7, whereas both can not yet compile all components of 0TL.

What does hybrid mean?
----------------------
Pure template meta programming is limited to computation on types, while C++'s core language performs its computation based on instances. Making the transition from the compile-time to the runtime world is always tricky. Therefore, it might be useful to make use of other static language features like `constexpr`.
Good examples for hybrid template containers are std::tuple and the boost::fusion containers.

Licensing
---------
Copyright (c) 2011, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
Distributed under the terms of the GPLv2 or newer
