
#ifndef BOOST_MPL_AUX_CONFIG_INTEGRAL_HPP_INCLUDED
#define BOOST_MPL_AUX_CONFIG_INTEGRAL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: integral.hpp 1944 2012-11-02 14:01:05Z moene $
// $Date: 2012-11-02 15:01:05 +0100 (Fri, 02 Nov 2012) $
// $Revision: 1944 $

#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

#if    !defined(BOOST_MPL_CFG_BCC_INTEGRAL_CONSTANTS) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE) \
    && BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x610))

#   define BOOST_MPL_CFG_BCC_INTEGRAL_CONSTANTS

#endif

#if    !defined(BOOST_MPL_CFG_NO_NESTED_VALUE_ARITHMETIC) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE) \
    && ( BOOST_WORKAROUND(BOOST_MSVC, <= 1300) \
        || BOOST_WORKAROUND(__EDG_VERSION__, <= 238) \
        )

#   define BOOST_MPL_CFG_NO_NESTED_VALUE_ARITHMETIC

#endif

#endif // BOOST_MPL_AUX_CONFIG_INTEGRAL_HPP_INCLUDED
