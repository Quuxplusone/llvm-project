//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: clang
// UNSUPPORTED: clang-15, clang-16
// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <array>
#include <complex>
#include <coroutine>
#include <deque>
#include <expected>
#include <forward_list>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include "test_macros.h"

template <class T>
constexpr bool is_tec = __is_trivially_equality_comparable(T);

template<class T>
struct Holder {
  T t;
};

// H* is a pointer type, so its `==` doesn't do ADL.
// HH is a class type that is safe to instantiate but still fatal to ADL.
// Even when instantiating e.g. `tuple<HH>`, we must never try to resolve HH's `==`
// unless the programmer specifically asks for it.
//
struct Incomplete;
using H = Holder<Incomplete>;
using HH = Holder<H*>;
static_assert(is_tec<H*>);

struct HView : public std::ranges::view_interface<HView> {
  H** begin() const;
  H** end() const;
};
struct HTupleView : public std::ranges::view_interface<HTupleView> {
  std::tuple<H*>* begin() const;
  std::tuple<H*>* end() const;
};

struct HHView : public std::ranges::view_interface<HHView> {
  HH* begin() const;
  HH* end() const;
};
struct HHTupleView : public std::ranges::view_interface<HHTupleView> {
  std::tuple<HH>* begin() const;
  std::tuple<HH>* end() const;
};

template<class T>
struct Adjust {
  static_assert(std::is_empty_v<T>);
  [[no_unique_address]] T t;
  int i;
  bool operator==(const Adjust&) const = default;
};

template<class R>
using IterOf = decltype(std::declval<R&>().begin());

static_assert(!is_tec<std::array<int, 0>>); // padding
static_assert(!is_tec<std::complex<int>>); // TODO
static_assert(!is_tec<std::coroutine_handle<>>); // TODO
static_assert(!is_tec<std::pair<int, char>>); // padding
static_assert(!is_tec<std::pair<int, int&>>); // reference member
static_assert(!is_tec<std::tuple<>>); // padding
static_assert(!is_tec<Adjust<std::tuple<>>>); // TODO
static_assert(!is_tec<std::tuple<int, int>>); // TODO
static_assert(!is_tec<std::tuple<int, char>>); // padding
static_assert(!is_tec<std::tuple<int, int&>>); // reference member

static_assert(!is_tec<std::array<H*, 1>>); // TODO
static_assert(!is_tec<std::deque<H*>::iterator>); // TODO
static_assert(!is_tec<std::deque<H*>::const_iterator>); // TODO
static_assert(!is_tec<std::forward_list<H*>::iterator>); // TODO
static_assert(!is_tec<std::forward_list<H*>::const_iterator>); // TODO
static_assert(!is_tec<std::list<H*>::iterator>); // TODO
static_assert(!is_tec<std::list<H*>::const_iterator>); // TODO
static_assert(!is_tec<std::map<H*,H*>::iterator>); // TODO
static_assert(!is_tec<std::map<H*,H*>::const_iterator>); // TODO
static_assert(!is_tec<std::multimap<H*,H*>::iterator>); // TODO
static_assert(!is_tec<std::multimap<H*,H*>::const_iterator>); // TODO
static_assert(!is_tec<std::multiset<H*>::iterator>); // TODO
static_assert(!is_tec<std::multiset<H*>::const_iterator>); // TODO
static_assert(!is_tec<std::set<H*>::iterator>); // TODO
static_assert(!is_tec<std::set<H*>::const_iterator>); // TODO
static_assert(!is_tec<std::pair<H*, H*>>); // TODO
static_assert(!is_tec<IterOf<std::ranges::elements_view<HTupleView, 0>>>); // TODO
static_assert(!is_tec<IterOf<std::ranges::iota_view<H**, H**>>>); // TODO
static_assert(!is_tec<std::tuple<H*>>); // TODO
static_assert(!is_tec<std::unique_ptr<H*>>); // TODO
static_assert(!is_tec<std::vector<H*>::iterator>); // TODO
static_assert(!is_tec<std::vector<H*>::const_iterator>); // TODO

#if TEST_STD_VER >= 23
static_assert(!is_tec<IterOf<std::ranges::zip_view<HView, HView>>>); // TODO
static_assert(is_tec<std::unexpected<H*>>);
#endif

void test_robust_against_adl(H *h, HH hh) {
  // Make sure each type can still be instantiated without error.
  {
    [[maybe_unused]] std::array<H*, 1> t1;
    [[maybe_unused]] std::deque<H*>::iterator t2;
    [[maybe_unused]] std::deque<H*>::const_iterator t3;
    [[maybe_unused]] std::forward_list<H*>::iterator t4;
    [[maybe_unused]] std::forward_list<H*>::const_iterator t5;
    [[maybe_unused]] std::list<H*>::iterator t6;
    [[maybe_unused]] std::list<H*>::const_iterator t7;
    [[maybe_unused]] std::map<H*,H*>::iterator t8;
    [[maybe_unused]] std::map<H*,H*>::const_iterator t9;
    [[maybe_unused]] std::multimap<H*,H*>::iterator t10;
    [[maybe_unused]] std::multimap<H*,H*>::const_iterator t11;
    [[maybe_unused]] std::multiset<H*>::iterator t12;
    [[maybe_unused]] std::multiset<H*>::const_iterator t13;
    [[maybe_unused]] std::set<H*>::iterator t14;
    [[maybe_unused]] std::set<H*>::const_iterator t15;
    [[maybe_unused]] std::pair<H*, H*> t16 = std::make_pair(h, h);
    [[maybe_unused]] std::pair<H*&, H*&> t17 = std::make_pair(std::ref(h), std::ref(h));
    [[maybe_unused]] auto t18 = std::views::elements<0>(HTupleView()).begin();
    [[maybe_unused]] auto t19 = std::views::iota(&h, &h).begin();
    [[maybe_unused]] std::tuple<H*> t20 = std::make_tuple(h);
    [[maybe_unused]] std::tuple<H*&> t21 = std::tie(h);
    [[maybe_unused]] std::unique_ptr<H*> t22 = nullptr;
    [[maybe_unused]] std::vector<H*>::iterator t23;
    [[maybe_unused]] std::vector<H*>::const_iterator t24;
#if TEST_STD_VER >= 23
    [[maybe_unused]] auto t25 = std::views::zip(HView(), HView()).begin();
    [[maybe_unused]] auto t26 = std::unexpected<H*>(h);
#endif
  }
  {
    [[maybe_unused]] std::array<HH, 1> t1;
    [[maybe_unused]] std::deque<HH>::iterator t2;
    [[maybe_unused]] std::deque<HH>::const_iterator t3;
    [[maybe_unused]] std::forward_list<HH>::iterator t4;
    [[maybe_unused]] std::forward_list<HH>::const_iterator t5;
    [[maybe_unused]] std::list<HH>::iterator t6;
    [[maybe_unused]] std::list<HH>::const_iterator t7;
    [[maybe_unused]] std::map<int,HH>::iterator t8;
    [[maybe_unused]] std::map<int,HH>::const_iterator t9;
    [[maybe_unused]] std::multimap<int,HH>::iterator t10;
    [[maybe_unused]] std::multimap<int,HH>::const_iterator t11;
    [[maybe_unused]] std::multiset<HH>::iterator t12;
    [[maybe_unused]] std::multiset<HH>::const_iterator t13;
    [[maybe_unused]] std::set<HH>::iterator t14;
    [[maybe_unused]] std::set<HH>::const_iterator t15;
    [[maybe_unused]] std::pair<HH, HH> t16 = std::make_pair(hh, hh);
    [[maybe_unused]] std::pair<HH&, HH&> t17 = std::make_pair(std::ref(hh), std::ref(hh));
    [[maybe_unused]] auto t18 = std::views::elements<0>(HHTupleView()).begin();
    [[maybe_unused]] auto t19 = std::views::iota(std::addressof(hh), std::addressof(hh)).begin();
    [[maybe_unused]] std::tuple<HH> t20 = std::make_tuple(hh);
    [[maybe_unused]] std::tuple<HH&> t21 = std::tie(hh);
    [[maybe_unused]] std::unique_ptr<HH> t22 = nullptr;
    [[maybe_unused]] std::vector<HH>::iterator t23;
    [[maybe_unused]] std::vector<HH>::const_iterator t24;
#if TEST_STD_VER >= 23
    [[maybe_unused]] auto t25 = std::views::zip(HHView(), HHView()).begin();
    [[maybe_unused]] auto t26 = std::unexpected<HH>(hh);
#endif
  }
}
