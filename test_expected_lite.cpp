// Copyright (C) 2013 Martin Moene.
//
// Implementation note: this version targets C++03.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea is based on the Expected<T> class template authored by
// Andrei Alexandrescu. The interface is based on the std::optional proposal
// authored by Fernando Cacciola and Andrzej Krzemienski and the
// Boost.Expected proposal authored by Pierre Talbot.

#include "expected_lite.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

struct caller {
    template <class T> caller(T fun) { fun(); }
};
# define CAT2(X, Y) X ## Y
# define CAT(X, Y) CAT2(X, Y)
# define TEST(NAME) caller CAT(__VAR, __LINE__) = []

enum  State
{
    sDefaultConstructed,
    sValueCopyConstructed,
    sValueMoveConstructed,
    sCopyConstructed,
    sMoveConstructed,
    sMoveAssigned,
    sCopyAssigned,
    sValueCopyAssigned,
    sValueMoveAssigned,
    sMovedFrom,
    sValueConstructed
};

struct OracleVal
{
    State s;
    int i;
    OracleVal(int i = 0) : s(sValueConstructed), i(i) {}

    bool operator==( OracleVal const & other ) const { return s==other.s && i==other.i; }
};

struct Oracle
{
    State s;
    OracleVal val;

    Oracle() : s(sDefaultConstructed) {}
    Oracle(const OracleVal& v) : s(sValueCopyConstructed), val(v) {}
    Oracle(OracleVal&& v) : s(sValueMoveConstructed), val(std::move(v)) {v.s = sMovedFrom;}
    Oracle(const Oracle& o) : s(sCopyConstructed), val(o.val) {}
    Oracle(Oracle&& o) : s(sMoveConstructed), val(std::move(o.val)) {o.s = sMovedFrom;}

    Oracle& operator=(const OracleVal& v) { s = sValueCopyConstructed; val = v; return *this; }
    Oracle& operator=(OracleVal&& v) { s = sValueMoveConstructed; val = std::move(v); v.s = sMovedFrom; return *this; }
    Oracle& operator=(const Oracle& o) { s = sCopyConstructed; val = o.val; return *this; }
    Oracle& operator=(Oracle&& o) { s = sMoveConstructed; val = std::move(o.val); o.s = sMovedFrom; return *this; }

    bool operator==( Oracle const & other ) const { return s == other.s && val == other.val;}
};

namespace ns = nonstd;

TEST(disengaged_ctor)
{
    ns::expected<int> o1;
    assert (!o1);

    ns::expected<int> o2 = ns::nullexp;
    assert (!o2);

    ns::expected<int> o3 = o2;
    assert (!o3);

    assert (o1 == ns::nullexp);
    assert (o1 == ns::expected<int>{} );
    assert (!o1);
    assert (bool(o1) == false);

    assert (o2 == ns::nullexp);
    assert (o2 == ns::expected<int>{});
    assert (!o2);
    assert (bool(o2) == false);

    assert (o3 == ns::nullexp);
    assert (o3 == ns::expected<int>{});
    assert (!o3);
    assert (bool(o3) == false);

    assert (o1 == o2);
    assert (o2 == o1);
    assert (o1 == o3);
    assert (o3 == o1);
    assert (o2 == o3);
    assert (o3 == o2);
};

TEST(value_ctor)
{
  OracleVal v;
  ns::expected<Oracle> oo1(v);
  assert (oo1 != ns::nullexp);
  assert (oo1 != ns::expected<Oracle>{});
  assert (oo1 == ns::expected<Oracle>{v});
  assert (!!oo1);
  assert (bool(oo1));
  // NA: assert (oo1->s == sValueCopyConstructed);
  assert (oo1->s == sMovedFrom);
//  assert (oo1->s == sMoveConstructed);
  assert (v.s == sValueConstructed);

//  ns::expected<Oracle> oo2(std::move(v));
//  assert (oo2 != ns::nullexp);
//  assert (oo2 != ns::expected<Oracle>{});
//  assert (oo2 == oo1);
//  assert (!!oo2);
//  assert (bool(oo2));
//  // NA: assert (oo2->s == sValueMoveConstructed);
//  assert (oo2->s == sMoveConstructed);
//  assert (v.s == sMovedFrom);

  {
      OracleVal v;
//      ns::expected<Oracle> oo1{ns::emplace, v};
      ns::expected<Oracle> oo1{v};
      assert (oo1 != ns::nullexp);
      assert (oo1 != ns::expected<Oracle>{});
      assert (oo1 == ns::expected<Oracle>{v});
      assert (!!oo1);
      assert (bool(oo1));
      assert (oo1->s == sValueCopyConstructed);
      assert (v.s == sValueConstructed);

//      ns::expected<Oracle> oo2{ns::emplace, std::move(v)};
//      assert (oo2 != ns::nullexp);
//      assert (oo2 != ns::expected<Oracle>{});
//      assert (oo2 == oo1);
//      assert (!!oo2);
//      assert (bool(oo2));
//      assert (oo2->s == sValueMoveConstructed);
//      assert (v.s == sMovedFrom);
  }
};


int main()
{
    return 0; // VC6
}

// cl -nologo -W3 -EHsc -Ideps test_expected_lite.cpp && test_expected_lite
// g++ -Wall -Wextra -std=c++03 -Ideps -o test_expected_lite.exe test_expected_lite.cpp && test_expected_lite
// g++ -Wall -Wextra -std=c++11 -o test_expected_lite.exe test_expected_lite.cpp && test_expected_lite
