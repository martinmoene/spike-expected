// Copyright (C) 2013 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ReturnCode Copyright (C) 2000 by Ken Hagan

#include "required.hpp"
#include <cassert>
#include <iostream>

nonstd::required<long> foo()
{
    return 77;
}

template <typename T>
std::ostream & operator<<( std::ostream & os, nonstd::required<T> arg )
{
    return os << ( (T) arg );
}

int main()
{
    using namespace nonstd;

    try
    {
        required<long> f = foo();
        required<long> b = foo();

        std::cout << "f: "; std::cout << f << std::endl;
        std::cout << "b: "; // std::cout << b << std::endl;

        // b throws:
    }
    catch ( std::exception const & e )
    {
        std::cout << "Error (exception): " << e.what() << std::endl;
    }
    catch ( long const & e )
    {
        std::cout << "Error (long): " << e << std::endl;
    }
    catch (...)
    {
        assert( false );
    }

    return 0; // VC6
}

// cl -nologo -W3 -EHsc -Ideps test_required.cpp && test_required
// g++ -Wall -Wextra -std=c++03 -Ideps -o test_required.exe test_required.cpp && test_required
// g++ -Wall -Wextra -std=c++11 -o test_required.exe test_required.cpp && test_required
