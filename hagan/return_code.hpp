// Copyright 2000 Ken Hagan
// Exploding return codes.
// https://groups.google.com/forum/#!msg/comp.lang.c++.moderated/BkZqPfoq3ys/H_PMR8Sat4oJ

#ifndef HAGAN_EXPLODING_RETURN_CODE_H_INCLUDED
#define HAGAN_EXPLODING_RETURN_CODE_H_INCLUDED

class return_code
{
    long m_rc;

public:
#if __cplusplus >= 201103L
    ~return_code() noexcept(false)
#else
    ~return_code()
#endif
    {
        if ( m_rc < 0 )
            throw m_rc;
    }

    return_code( long rc )
    : m_rc( rc ) {}

    operator long ()
    {
        long rc = m_rc;
        m_rc = -1;
        return rc;
    }
};

#endif // HAGAN_EXPLODING_RETURN_CODE_H_INCLUDED
