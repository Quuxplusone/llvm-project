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
#include <exception>
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

struct T {
  int i;
  bool operator==(const T&) const; // non-defaulted
};

template<class R>
using IterOf = decltype(std::declval<R&>().begin());

// Non-trivial cases that should remain non-trivial
static_assert(!is_tec<std::array<int, 0>>); // padding
static_assert(!is_tec<std::array<T, 1>>); // T is non-tec
static_assert(!is_tec<std::pair<int, T>>); // T is non-tec
static_assert(!is_tec<std::pair<int, char>>); // padding
static_assert(!is_tec<std::pair<int, int&>>); // reference member
static_assert(!is_tec<std::shared_ptr<int>>); // ctrl pointer isn't salient
static_assert(!is_tec<std::shared_ptr<int[]>>); // ctrl pointer isn't salient
static_assert(!is_tec<std::tuple<>>); // padding
static_assert(!is_tec<std::tuple<T>>); // T is non-tec
static_assert(!is_tec<std::tuple<int, char>>); // padding
static_assert(!is_tec<std::tuple<int, int&>>); // reference member
#if TEST_STD_VER >= 23
static_assert(!is_tec<std::unexpected<T>>); // T is non-tec
#endif
static_assert(!is_tec<std::unique_ptr<T, void(*)(T*)>>); // Deleter isn't salient
static_assert(!is_tec<std::unique_ptr<T[], void(*)(T*)>>); // Deleter isn't salient

// Cases that seemingly can't be trivial
static_assert(!is_tec<std::array<int, 1>>);
static_assert(!is_tec<std::pair<int, int>>);
static_assert(!is_tec<std::tuple<int, int>>);
#if TEST_STD_VER >= 23
static_assert(!is_tec<std::unexpected<int>>);
#endif

// Cases that should be trivial but aren't yet implemented
static_assert(!is_tec<std::deque<T>::iterator>); // TODO
static_assert(!is_tec<std::deque<T>::const_iterator>); // TODO
static_assert(!is_tec<std::forward_list<T>::iterator>); // TODO
static_assert(!is_tec<std::forward_list<T>::const_iterator>); // TODO
static_assert(!is_tec<std::map<int, T>::iterator>); // TODO
static_assert(!is_tec<std::map<int, T>::const_iterator>); // TODO
static_assert(!is_tec<std::multimap<int, T>::iterator>); // TODO
static_assert(!is_tec<std::multimap<int, T>::const_iterator>); // TODO
static_assert(!is_tec<std::multiset<T>::iterator>); // TODO
static_assert(!is_tec<std::multiset<T>::const_iterator>); // TODO
static_assert(!is_tec<std::set<T>::iterator>); // TODO
static_assert(!is_tec<std::set<T>::const_iterator>); // TODO
static_assert(!is_tec<std::tuple<int, int>>); // TODO
static_assert(!is_tec<Adjust<std::tuple<>>>); // TODO
static_assert(!is_tec<std::unique_ptr<T>>); // TODO
static_assert(!is_tec<std::unique_ptr<T[]>>); // TODO

// Simple cases with `int` or `T`
static_assert(is_tec<std::complex<int>>);
static_assert(is_tec<std::coroutine_handle<>>);
static_assert(is_tec<std::coroutine_handle<T>>);
static_assert(is_tec<std::exception_ptr>);
static_assert(is_tec<std::list<T>::iterator>);
static_assert(is_tec<std::list<T>::const_iterator>);
static_assert(is_tec<std::vector<T>::iterator>);
static_assert(is_tec<std::vector<T>::const_iterator>);

// Complicated cases with `H*` that work today
#if TEST_STD_VER >= 23
static_assert(!is_tec<IterOf<std::ranges::elements_view<HTupleView, 0>>>); // TODO
static_assert(!is_tec<IterOf<std::ranges::iota_view<H**, H**>>>); // TODO
static_assert(!is_tec<IterOf<std::ranges::zip_view<HView, HView>>>); // TODO
#endif

void test_robust_against_adl(H *h, HH hh) {
  // Make sure each type can still be instantiated without error.
  {
    [[maybe_unused]] auto t1 = std::array<H*, 1>();
    [[maybe_unused]] auto t2 = std::coroutine_handle<H*>();
    [[maybe_unused]] auto t3 = std::deque<H*>::iterator();
    [[maybe_unused]] auto t4 = std::deque<H*>::const_iterator();
    [[maybe_unused]] auto t5 = std::forward_list<H*>::iterator();
    [[maybe_unused]] auto t6 = std::forward_list<H*>::const_iterator();
    [[maybe_unused]] auto t7 = std::list<H*>::iterator();
    [[maybe_unused]] auto t8 = std::list<H*>::const_iterator();
    [[maybe_unused]] auto t9 = std::map<H*,H*>::iterator();
    [[maybe_unused]] auto t10 = std::map<H*,H*>::const_iterator();
    [[maybe_unused]] auto t11 = std::multimap<H*,H*>::iterator();
    [[maybe_unused]] auto t12 = std::multimap<H*,H*>::const_iterator();
    [[maybe_unused]] auto t13 = std::multiset<H*>::iterator();
    [[maybe_unused]] auto t14 = std::multiset<H*>::const_iterator();
    [[maybe_unused]] auto t15 = std::set<H*>::iterator();
    [[maybe_unused]] auto t16 = std::set<H*>::const_iterator();
    [[maybe_unused]] auto t17 = std::pair<H*, H*>();
    [[maybe_unused]] std::pair<H*&, H*&> t18 = std::make_pair(std::ref(h), std::ref(h));
    [[maybe_unused]] auto t19 = std::views::elements<0>(HTupleView()).begin();
    [[maybe_unused]] auto t20 = std::views::iota(&h, &h).begin();
    [[maybe_unused]] auto t21 = std::tuple<H*>();
    [[maybe_unused]] std::tuple<H*&> t22 = std::tie(h);
    [[maybe_unused]] auto t23 = std::unique_ptr<H*>();
    [[maybe_unused]] auto t24 = std::vector<H*>::iterator();
    [[maybe_unused]] auto t25 = std::vector<H*>::const_iterator();
#if TEST_STD_VER >= 23
    [[maybe_unused]] auto t26 = std::views::zip(HView(), HView()).begin();
    [[maybe_unused]] auto t27 = std::unexpected<H*>(nullptr);
#endif
  }
  {
    [[maybe_unused]] auto t1 = std::array<HH, 1>();
    [[maybe_unused]] auto t2 = std::coroutine_handle<HH>();
    [[maybe_unused]] auto t3 = std::deque<HH>::iterator();
    [[maybe_unused]] auto t4 = std::deque<HH>::const_iterator();
    [[maybe_unused]] auto t5 = std::forward_list<HH>::iterator();
    [[maybe_unused]] auto t6 = std::forward_list<HH>::const_iterator();
    [[maybe_unused]] auto t7 = std::list<HH>::iterator();
    [[maybe_unused]] auto t8 = std::list<HH>::const_iterator();
    [[maybe_unused]] auto t9 = std::map<int,HH>::iterator();
    [[maybe_unused]] auto t10 = std::map<int,HH>::const_iterator();
    [[maybe_unused]] auto t11 = std::multimap<int,HH>::iterator();
    [[maybe_unused]] auto t12 = std::multimap<int,HH>::const_iterator();
    [[maybe_unused]] auto t13 = std::multiset<HH>::iterator();
    [[maybe_unused]] auto t14 = std::multiset<HH>::const_iterator();
    [[maybe_unused]] auto t15 = std::set<HH>::iterator();
    [[maybe_unused]] auto t16 = std::set<HH>::const_iterator();
    [[maybe_unused]] auto t17 = std::pair<HH, HH>();
    [[maybe_unused]] std::pair<HH&, HH&> t18 = std::make_pair(std::ref(hh), std::ref(hh));
    [[maybe_unused]] auto t19 = std::views::elements<0>(HHTupleView()).begin();
    [[maybe_unused]] auto t20 = std::views::iota(std::addressof(hh), std::addressof(hh)).begin();
    [[maybe_unused]] auto t21 = std::tuple<HH>();
    [[maybe_unused]] std::tuple<HH&> t22 = std::tie(hh);
    [[maybe_unused]] auto t23 = std::unique_ptr<HH>();
    [[maybe_unused]] auto t24 = std::vector<HH>::iterator();
    [[maybe_unused]] auto t25 = std::vector<HH>::const_iterator();
#if TEST_STD_VER >= 23
    [[maybe_unused]] auto t26 = std::views::zip(HHView(), HHView()).begin();
    [[maybe_unused]] auto t27 = std::unexpected<HH>(hh);
#endif
  }
}
