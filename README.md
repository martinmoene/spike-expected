expected\<T\>
===========

Variations on Expected\<T\> by Andrei Alexandrescu.

Feature Comparison
------------------

|Feature                 |std::pair |std::optional |nonstd::expected |Boost.Expected |Nonco expected |Andrei Expected |
|------------------------|----------|--------------|-----------------|---------------|---------------|----------------|
|More information        |          | see [1]      | this work       | see [4]       | see [3]       | see [5]        |
|                        |          |              |                 |               |               |                |
|Disengaged information  | possible | no           | yes             | yes           | yes           | yes            |
|                        |          |              |                 |               |               |                |
|C++03                   | yes      | no           | yes             | yes           | no            | no             |
|C++11                   | yes      | yes          | no/not yet      | yes           | yes           | yes            |
|                        |          |              |                 |               |               |                |
|DefaulConstructible     | T param  | yes          | yes             | no            | no            | no             |
|Throws on disengaged use| no       | yes, value() | yes, value()    | yes, get()    | yes, get()    | yes, get()     |
|Relational operators    | no       | yes          | yes             | no            | no            | no             |
|References              | wrap     | yes          | no/not yet      | no            | yes           | no             |
|Chaining                | no       | no           | maybe, separate | maybe         | no            | no             |



References
----------

[1] Fernando Cacciola and Andrzej Krzemieński. [A proposal to add a utility class to represent optional objects (Revision 4)](http://isocpp.org/files/papers/N3672.html). ISO/IEC JTC1 SC22 WG21 N3672 2013-04-19.

[2] Andrzej Krzemieński, [Optional library implementation in C++11](https://github.com/akrzemi1/Optional/).

[3] Anto Nonco. [Extending expected<T> to deal with references](http://anto-nonco.blogspot.nl/2013/03/extending-expected-to-deal-with.html). 27 May 2013.

[4] Pierre Talbot. [Boost.Expected. Unofficial Boost candidate](http://www.google-melange.com/gsoc/proposal/review/google/gsoc2013/trademark/25002). 5 May 2013. [GitHub](https://github.com/TrademarkPewPew/Boost.Expected), [GSoC 2013 Proposal](http://www.google-melange.com/gsoc/proposal/review/google/gsoc2013/trademark/25002), [boost@lists.boost.org](http://permalink.gmane.org/gmane.comp.lib.boost.devel/240056 ).

[5] Andrei Alexandrescu. Systematic Error Handling in C++. Prepared for The C++and Beyond Seminar 2012. [Video](http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C). [Slides](http://sdrv.ms/RXjNPR).

[6] Andrei Alexandrescu. [Choose your Poison: Exceptions or Error Codes? (PDF)](http://accu.org/content/conf2007/Alexandrescu-Choose_Your_Poison.pdf). ACCU
Conference 2007.

[7] Andrei Alexandrescu. [The Power of None (PPT)](http://nwcpp.org/static/talks/2006/The_Power_of_None.ppt). Northwest C++ Usersâ€™ Group. [May 17th, 2006](http://nwcpp.org/may-2006.html).

[8] Jon Jagger. [A Return Type That Doesnâ€™t Like Being Ignored](http://accu.org/var/uploads/journals/overload53-FINAL.pdf#page=18). Overload issue 53, February 2003.

[9] Andrei Alexandrescu. [Error Handling in C++: Are we inching towards a total solution?](http://accu.org/index.php/conferences/2002/speakers2002). ACCU Conference 2002.

[10] Ken Hagan et al. [Exploding return codes](https://groups.google.com/d/msg/comp.lang.c++.moderated/BkZqPfoq3ys/H_PMR8Sat4oJ). comp.lang.c++.moderated. 11 February 2000.


C++11 compiler support
----------------------

* Scott Meyers. [Summary of C++11 Feature Availability in gcc and MSVC](http://www.aristeia.com/C++11/C++11FeatureAvailability.htm). 31 January 2012.

* C++ Rocks. [C++11 compiler support shootout: Visual Studio, GCC, Clang, Intel](http://cpprocks.com/c11-compiler-support-shootout-visual-studio-gcc-clang-intel/). 14 March 2013.


Other
-----

* Current C++ Standard Working Draft (june 2013). [N3690 (PDF)](http://isocpp.org/files/papers/N3690.pdf).

* Herb Sutter. [Style Case Study #3: Construction Unions](http://www.gotw.ca/gotw/085.htm). GotW #85. 2009

* Kevin T. Manley. [Using Constructed Types in C++ Unions](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/0208/manley/manley.htm). C/C++ Users Journal, 20(8), August 2002.

* Programming in Lua, Section [Relational Metamethods](http://www.lua.org/pil/13.2.html).
