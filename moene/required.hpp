// Copyright 2000 Ken Hagan
// Exploding return codes.
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
        if ( ! m_used )
            throw m_value;
    }

    required( value_type const & v )
    : m_value( v )
    , m_used( false )
    {}

    // move constructor
    required( required & other )
    : m_value( other.m_value )
    , m_used( other.m_used )
    {
        other.m_used = true;
    }

    operator value_type() const
    {
        m_used = true;
        return m_value;
    }

private:
    value_type m_value;
    mutable bool m_used;
};

} // namespace nonstd

#endif // NONSTD_REQUIRED_H_INCLUDED
