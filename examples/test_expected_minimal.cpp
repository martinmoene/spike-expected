// Copyright (C) 2013 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "expected_minimal.hpp"

#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        expected<int> e0;
        expected<int> ei{ 7 };
        expected<int> ee{ nullexp,
            std::make_exception_ptr(std::runtime_error("disengaged")) };

        std::cout << "ei.value(): "; std::cout << ei.value() << std::endl;
        std::cout << "ee.value(): "; std::cout << ee.value() << std::endl;
    }
    catch ( std::exception const & e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

// g++ -Wall -Wextra -std=c++11 -o test_expected_minimal.exe test_expected_minimal.cpp && test_expected_minimal
