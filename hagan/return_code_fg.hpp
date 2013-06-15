// Copyright 2000 Francis Glassborow
// Exploding return codes.
// https://groups.google.com/forum/#!msg/comp.lang.c++.moderated/BkZqPfoq3ys/H_PMR8Sat4oJ

// Note: without a move constructor you risk throwing in the destructor while
//       an exception is already in flight, resulting in a call to std::terminate().

#ifndef GLASSBOROW_EXPLODING_RETURN_CODE_H_INCLUDED
#define GLASSBOROW_EXPLODING_RETURN_CODE_H_INCLUDED

#include <string>
#include <iostream>

struct trace
{
    std::string text;
    trace(std::string text)
    : text(text)
    {
        std::cout << "> " << text << std::endl;
    }
    ~trace()
    {
        std::cout << "< " << text << std::endl;
    }
};

class return_code
{
    long rc_m;
    bool ignored;

public:
    return_code(long rc)
    : rc_m(rc)
    , ignored(true)
    {
        trace t("rc(long)");
    }

#if __cplusplus >= 201103L
    ~return_code() noexcept(false)
#else
    ~return_code()
#endif
    {
        trace t("~rc()");
        if (ignored)
        {
            throw rc_m;
        }
    }

    operator long ()
    {
        trace t("op long()");
        ignored = false;
        return rc_m;
    }
};

#endif // GLASSBOROW_EXPLODING_RETURN_CODE_H_INCLUDED
