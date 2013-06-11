//
// expected - variations on Expected<T> by Andrei Alexandrescu.
//
// Written in 2013 by Martin Moene <martin.moene@gmail.com>
//

#ifndef NONSTD_STATIC_VISITOR_H_INCLUDED
#define NONSTD_STATIC_VISITOR_H_INCLUDED

namespace nonstd {

template <typename T>
class static_visitor
{
public:
    typedef T result_type;
};

}

#endif // NONSTD_STATIC_VISITOR_H_INCLUDED
