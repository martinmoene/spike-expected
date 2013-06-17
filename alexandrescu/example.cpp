// Copyright (C) 2012 Andrei Alexandrescu.

#include "expected.h"
#include <string>
#include <cassert>

// Use example(callee)

Expected<int> parseInt( const std::string & s )
{
    int result = 0;;
    bool nonDigit = false;
    bool tooManyDigits = false;
    // ...

    if( nonDigit )
    {
        return Expected<int>::fromException( std::invalid_argument( "not a number" ) );
    }

    // ...

    if( tooManyDigits )
    {
        return Expected<int>::fromException( std::out_of_range( "overflow" ) );
    }

    // ...

    return result;
}

// Use example(caller)

//Caller

using namespace std;

string readline();

void caller()
{
    string s = readline();
    auto x = parseInt( s ).get();   //throw on error
    auto y = parseInt( s );         //won’t throw

    if( !y.valid() )
    {
        //handle locally

        if( y.hasException<std::invalid_argument>() )
        {
            // nodigits
            // ...
        }

        y.get(); //just"re"throw
    }
}
