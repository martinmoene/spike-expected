
#ifndef BOOST_MPL_AUX_ARITY_HPP_INCLUDED
#define BOOST_MPL_AUX_ARITY_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: arity.hpp 1944 2012-11-02 14:01:05Z moene $
// $Date: 2012-11-02 15:01:05 +0100 (Fri, 02 Nov 2012) $
// $Revision: 1944 $

#include <boost/mpl/aux_/config/dtp.hpp>

#if defined(BOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)

#   include <boost/mpl/aux_/nttp_decl.hpp>
#   include <boost/mpl/aux_/config/static_constant.hpp>

namespace boost { namespace mpl { namespace aux {

// agurt, 15/mar/02: it's possible to implement the template so that it will 
// "just work" and do not require any specialization, but not on the compilers
// that require the arity workaround in the first place
template< typename F, BOOST_MPL_AUX_NTTP_DECL(int, N) >
struct arity
{
    BOOST_STATIC_CONSTANT(int, value = N);
};

}}}

#endif // BOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES

#endif // BOOST_MPL_AUX_ARITY_HPP_INCLUDED
