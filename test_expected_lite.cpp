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
//  assert (oo1->s == sMovedFrom);
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
//      assert (oo1->s == sValueCopyConstructed);
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

TEST(value_or)
{
  nonstd::expected<int> oi = 1;
  int i = oi.value_or(0);
  assert (i == 1);

  oi = nonstd::nullexp;
  assert (oi.value_or(3) == 3);

  nonstd::expected<std::string> os{"AAA"};
  assert (os.value_or("BBB") == "AAA");
  os = nonstd::nullexp ; // {};
  assert (os.value_or("BBB") == "BBB");
};

TEST(mixed_order)
{
  using namespace nonstd;

  expected<int> oN {nullexp};
  expected<int> o0 {0};
  expected<int> o1 {1};

  assert ( (oN <   0));
  assert ( (oN <   1));
  assert (!(o0 <   0));
  assert ( (o0 <   1));
  assert (!(o1 <   0));
  assert (!(o1 <   1));

  assert (!(oN >=  0));
  assert (!(oN >=  1));
  assert ( (o0 >=  0));
  assert (!(o0 >=  1));
  assert ( (o1 >=  0));
  assert ( (o1 >=  1));

  assert (!(oN >   0));
  assert (!(oN >   1));
  assert (!(o0 >   0));
  assert (!(o0 >   1));
  assert ( (o1 >   0));
  assert (!(o1 >   1));

  assert ( (oN <=  0));
  assert ( (oN <=  1));
  assert ( (o0 <=  0));
  assert ( (o0 <=  1));
  assert (!(o1 <=  0));
  assert ( (o1 <=  1));

  assert ( (0 >  oN));
  assert ( (1 >  oN));
  assert (!(0 >  o0));
  assert ( (1 >  o0));
  assert (!(0 >  o1));
  assert (!(1 >  o1));

  assert (!(0 <= oN));
  assert (!(1 <= oN));
  assert ( (0 <= o0));
  assert (!(1 <= o0));
  assert ( (0 <= o1));
  assert ( (1 <= o1));

  assert (!(0 <  oN));
  assert (!(1 <  oN));
  assert (!(0 <  o0));
  assert (!(1 <  o0));
  assert ( (0 <  o1));
  assert (!(1 <  o1));

  assert ( (0 >= oN));
  assert ( (1 >= oN));
  assert ( (0 >= o0));
  assert ( (1 >= o0));
  assert (!(0 >= o1));
  assert ( (1 >= o1));
};

struct BadRelops
{
  int i;
};

constexpr bool operator<(BadRelops a, BadRelops b) { return a.i < b.i; }
constexpr bool operator>(BadRelops a, BadRelops b) { return a.i < b.i; } // intentional error!

TEST(bad_relops)
{
  using namespace nonstd;
  BadRelops a{1}, b{2};
  assert (a < b);
  assert (a > b);

  expected<BadRelops> oa = a, ob = b;
  assert (oa < ob);
  assert (!(oa > ob));

  assert (oa < b);
  assert (oa > b);

//  expected<BadRelops&> ra = a, rb = b;
//  assert (ra < rb);
//  assert (!(ra > rb));
//
//  assert (ra < b);
//  assert (ra > b);
};

//
//TEST(mixed_equality)
//{
//  using namespace nonstd;
//
//  assert (make_expected(0) == 0);
//  assert (make_expected(1) == 1);
//  assert (make_expected(0) != 1);
//  assert (make_expected(1) != 0);
//
//  expected<int> oN {nullexp};
//  expected<int> o0 {0};
//  expected<int> o1 {1};
//
//  assert (o0 ==  0);
//  assert ( 0 == o0);
//  assert (o1 ==  1);
//  assert ( 1 == o1);
//  assert (o1 !=  0);
//  assert ( 0 != o1);
//  assert (o0 !=  1);
//  assert ( 1 != o0);
//
//  assert ( 1 != oN);
//  assert ( 0 != oN);
//  assert (oN !=  1);
//  assert (oN !=  0);
//  assert (!( 1 == oN));
//  assert (!( 0 == oN));
//  assert (!(oN ==  1));
//  assert (!(oN ==  0));
//
//  std::string cat{"cat"}, dog{"dog"};
//  expected<std::string> oNil{}, oDog{"dog"}, oCat{"cat"};
//
//  assert (oCat ==  cat);
//  assert ( cat == oCat);
//  assert (oDog ==  dog);
//  assert ( dog == oDog);
//  assert (oDog !=  cat);
//  assert ( cat != oDog);
//  assert (oCat !=  dog);
//  assert ( dog != oCat);
//
//  assert ( dog != oNil);
//  assert ( cat != oNil);
//  assert (oNil !=  dog);
//  assert (oNil !=  cat);
//  assert (!( dog == oNil));
//  assert (!( cat == oNil));
//  assert (!(oNil ==  dog));
//  assert (!(oNil ==  cat));
//};

int main()
{
    return 0; // VC6
}

// cl -nologo -W3 -EHsc -Ideps test_expected_lite.cpp && test_expected_lite
// g++ -Wall -Wextra -std=c++03 -Ideps -o test_expected_lite.exe test_expected_lite.cpp && test_expected_lite
// g++ -Wall -Wextra -std=c++11 -o test_expected_lite.exe test_expected_lite.cpp && test_expected_lite
