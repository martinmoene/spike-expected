// Copyright (C) 2013 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ReturnCode Copyright (C) 2000 by Ken Hagan

#include "return_code.hpp"
#include <cassert>
#include <iostream>

return_code foo()
{
    return 77;
}

std::ostream & operator<<( std::ostream & os, return_code arg )
{
    return os << ( (long) arg );
}

int main()
{
    try
    {
        return_code f = foo();
        return_code b = foo();

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

// cl -nologo -W3 -EHsc -Ideps test_return_code.cpp && test_return_code
// g++ -Wall -Wextra -std=c++03 -Ideps -o test_return_code.exe test_return_code.cpp && test_return_code
// g++ -Wall -Wextra -std=c++11 -o test_return_code.exe test_return_code.cpp && test_return_code
