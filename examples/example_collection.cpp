// Copyright (C) 2013 Martin Moene.
//
// Implementation note: this version targets C++03.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "expected_lite.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

nonstd::expected<int> foo()
{
    return rand() % 100;
}

nonstd::expected<int> bar()
{
    return nonstd::expected<int>(
        nonstd::nullexp, std::make_exception_ptr( std::runtime_error( "disengaged" ) ) );
}

template <typename T, typename E>
std::ostream & operator<<( std::ostream & os, nonstd::expected<T,E> const & arg )
{
    if ( arg )
        return os << *arg << ", ";
    else
        return os << "#, ";
}

template <typename Collection>
void print( Collection const & c )
{
    for( auto elem : c )
    {
        std::cout << elem;
    }
    std::cout << std::endl;
}

int main()
{
    std::vector< nonstd::expected<int> > coll;

    coll.push_back( foo() );
    coll.push_back( foo() );
    coll.push_back( bar() );
    coll.push_back( foo() );
    coll.push_back( foo() );

    print( coll );
    std::sort( coll.begin(), coll.end() );
    print( coll );
}

// g++ -Wall -Wextra -std=c++11 -I.. -o example_collection.exe example_collection.cpp && example_collection
