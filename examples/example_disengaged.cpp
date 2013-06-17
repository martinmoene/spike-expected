// Copyright (C) 2013 Martin Moene.
//
// Implementation note: this version targets C++03.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "expected_lite.hpp"
#include <iostream>

nonstd::expected<int> foo()
{
    return 77;
}

nonstd::expected<int> bar()
{
    return nonstd::expected<int>(
        nonstd::nullexp, std::make_exception_ptr( std::runtime_error( "disengaged" ) ) );
}

template <typename T, typename E>
std::ostream & operator<<( std::ostream & os, nonstd::expected<T,E> const & arg )
{
    return os << arg.value();
}

int main()
{
    using namespace nonstd;

    try
    {
        expected<int> f = foo();
        expected<int> b = bar();    // propagate error via b

        std::cout << "f: "; std::cout << f << std::endl;
        std::cout << "b: "; std::cout << b << std::endl;

        // using value of b yields a std::runtime_error:
    }
    catch ( std::runtime_error const & e )
    {
        std::cout << "Error (runtime_error): " << e.what() << std::endl;
    }
    catch ( std::exception const & e )
    {
        std::cout << "Error (exception): " << e.what() << std::endl;
    }
    catch (...)
    {
        assert( false );
    }

    return 0; // VC6
}

// cl -nologo -W3 -EHsc -I.. -I../deps example_disengaged.cpp && example_disengaged
// g++ -Wall -Wextra -std=c++03 -I.. -I../deps -o example_disengaged.exe example_disengaged.cpp && example_disengaged
// g++ -Wall -Wextra -std=c++11 -I.. -o example_disengaged.exe example_disengaged.cpp && example_disengaged
