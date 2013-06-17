// Copyright (C) 2013 Martin Moene.
//
// Implementation note: this version targets C++03.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define has_trivial_destructor is_trivially_destructible
#include "../../Spikes/Optional/optional.hpp"

//#include "../alexandrescu/expected.hpp"
//#include "../nonco/expected.hpp"
//#include "expected_lite.hpp"
//#include "expected.hpp"
#include <iostream>

struct Guard
{
    std::string val;
    Guard() : val{} {}
    explicit Guard(std::string s, int = 0) : val(s) {}

    Guard(const Guard&) = delete;
    Guard(Guard&&) = delete;
    void operator=(const Guard&) = delete;
    void operator=(Guard&&) = delete;
};

int main()
{
    // std::optional<>
    namespace tr2 = std::experimental;

    tr2::optional< Guard > oge{ tr2::emplace, "guard" };
    tr2::optional< Guard > og;
    og.emplace( "guard" );

    // Andrei's Expected<>
//    Expected< Guard > ege{ "guard" };
//    Expected< Guard > eg;
//    eg.emplace( "guard" );

    // Nonco's expected<>
//    expected< Guard > ege{ "guard" };
//    expected< Guard > eg;
//    eg.emplace( "guard" );

    // nonstd::expected<>
//    using namespace nonstd;
//    expected< Guard > ege{ "guard" };
//    expected< Guard > eg;
//    eg.emplace( "guard" );
}

// g++ -Wall -Wextra -std=c++11 -I.. -o example_inplace.exe example_inplace.cpp && example_inplace
