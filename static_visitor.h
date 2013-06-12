// Copyright (C) 2013 Martin Moene <martin.moene@gmail.com>
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
