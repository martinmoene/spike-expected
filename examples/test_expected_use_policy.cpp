// Copyright (C) 2013 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "expected_use_policy.hpp"

#include <iostream>

int main()
{
    try
    {
        expected<int> ei{ 77 };

        std::cout << "not using ei.value(): \n";
    }
    catch ( std::exception const & e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    try
    {
        expected<int, require_policy> er{ 777 };

        std::cout << "not using er.value(): ";
    }
    catch ( std::exception const & e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

// g++ -Wall -Wextra -std=c++11 -o test_expected_use_policy.exe test_expected_use_policy.cpp && test_expected_use_policy
