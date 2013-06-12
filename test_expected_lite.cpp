// Copyright (C) 2013 Martin Moene.
//
// Implementation note: this version targets C++03.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea is based on the Expected<T> class template authored by
// Andrei Alexandrescu. The interface is based on the std::optional proposal
// authored by Fernando Cacciola and Andrzej Krzemienski and the
// Boost.Expected proposal authored by Pierre Talbot.

#define NONSTD_EXPECTED_DEFINE_ASSIGNMENT

#include "expected_lite.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

struct S
{
    S(std::string n) : n(n)     { std::cout << n << ": S()" << std::endl; }
    S(S const & r) : n(r.n+"@") { std::cout << n << ": S(S const &)" << std::endl; }
   ~S()                         { std::cout << n << ": ~S()" << std::endl; }

   const std::string n;
};

int main()
{
    using namespace nonstd;

    try
    {
        expected<int> ei = 33;
        std::cout << "ei-33: " << *ei << std::endl;

        int a = ei.value();
        int & b = ei.value();
        int & c = *ei;
        std::cout << "ei.value_or(22): " << ei.value_or(22) << std::endl;

        b = 77;
        std::cout << "ei-77: " << *ei << std::endl;
        std::cout << "c    : " << *ei << std::endl;

        expected<int> ek( 99 );
        std::cout << "ek   : " << *ek << std::endl;
#ifdef NONSTD_EXPECTED_DEFINE_ASSIGNMENT
        ek = ei;
        std::cout << "ek=ei: " << *(ek = ei) << std::endl;
#endif
//        std::exception_ptr p = ei.error();

        expected< S > es1 ( S("es1") );
        expected< S > es2 = S("es2");

        expected<int> ee1 ( nullexp, std::make_exception_ptr( std::runtime_error("ee1") ) );

        assert( ! !!ee1 );
        std::cout << "ee1.value_or(22): " << ee1.value_or(22) << std::endl;

        int & x = *ee1;
    }
    catch ( std::runtime_error const & e )
    {
        std::cout << "std::runtime_error: " << e.what() << std::endl;
    }
    catch ( std::exception const & e )
    {
        std::cout << "std::exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        assert( false );
    }

    return 0; // VC6
}

// cl -nologo -W3 -EHsc -Ideps test_expected_lite.cpp && test_expected_lite
// g++ -Wall -Wextra -std=c++03 -Ideps -o test_expected_lite.exe test_expected_lite.cpp && test_expected_lite
// g++ -Wall -Wextra -std=c++11 -o test_expected_lite.exe test_expected_lite.cpp && test_expected_lite
