// Copyright (C) 2013 Martin Moene.
//
// Implementation note: this version targets C++03 and C++11.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on exploding return codes by Ken Hagan.
// https://groups.google.com/forum/#!msg/comp.lang.c++.moderated/BkZqPfoq3ys/H_PMR8Sat4oJ

#ifndef NONSTD_REQUIRED_H_INCLUDED
#define NONSTD_REQUIRED_H_INCLUDED

namespace nonstd {

template <typename T>
class required
{
public:
    typedef T value_type;

#if __cplusplus >= 201103L
    ~required() noexcept(false)
#else
    ~required()
#endif
    {
        if ( m_ignored )
            throw m_value;
    }

    required( value_type const & v )
    : m_value( v )
    , m_ignored( true )
    {}

    // move "ignored" state unconditionally, C++03, C++11
    required( required const & other )
    : m_value( other.m_value )
    , m_ignored( other.m_ignored )
    {
        other.m_ignored = false;
    }

    operator value_type() const
    {
        m_ignored = false;
        return m_value;
    }

private:
    value_type m_value;
    mutable bool m_ignored;
};

} // namespace nonstd

#endif // NONSTD_REQUIRED_H_INCLUDED
