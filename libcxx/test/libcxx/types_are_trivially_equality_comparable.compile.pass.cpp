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

template<class T> struct Holder { T t; };
struct Incomplete;
using H = Holder<Incomplete>;
struct HView : public std::ranges::view_interface<HView> {
  H** begin() const;
  H** end() const;
};
struct TupleView : public std::ranges::view_interface<HView> {
  std::tuple<H*>* begin() const;
  std::tuple<H*>* end() const;
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
static_assert(!is_tec<std::array<H*, 1>>); // TODO
static_assert(!is_tec<std::complex<int>>); // TODO
static_assert(!is_tec<std::coroutine_handle<>>); // TODO
static_assert(!is_tec<std::forward_list<H*>::iterator>); // TODO
static_assert(!is_tec<std::forward_list<H*>::const_iterator>); // TODO
static_assert(!is_tec<std::list<H*>::iterator>); // TODO
static_assert(!is_tec<std::list<H*>::const_iterator>); // TODO
static_assert(!is_tec<std::map<H*, H*>::iterator>); // TODO
static_assert(!is_tec<std::multimap<H*, H*>::iterator>); // TODO
static_assert(!is_tec<std::multiset<H*>::iterator>); // TODO
static_assert(!is_tec<std::pair<H*, H*>>); // TODO
static_assert(!is_tec<std::pair<int, char>>); // padding
static_assert(!is_tec<std::pair<int, int&>>); // reference member
static_assert(!is_tec<IterOf<std::ranges::elements_view<TupleView, 0>>>); // TODO
static_assert(!is_tec<IterOf<std::ranges::iota_view<H**, H**>>>); // TODO
static_assert(!is_tec<std::set<H*>::iterator>); // TODO
static_assert(!is_tec<std::tuple<>>); // padding
static_assert(!is_tec<Adjust<std::tuple<>>>); // TODO
static_assert(!is_tec<std::tuple<H*>>); // TODO
static_assert(!is_tec<std::tuple<int, int>>); // TODO
static_assert(!is_tec<std::tuple<int, char>>); // padding
static_assert(!is_tec<std::tuple<int, int&>>); // reference member
static_assert(!is_tec<std::unique_ptr<H*>>); // TODO
static_assert(!is_tec<std::vector<H*>::iterator>); // TODO
static_assert(!is_tec<std::vector<H*>::const_iterator>); // TODO

#if TEST_STD_VER >= 23
static_assert(!is_tec<IterOf<std::ranges::zip_view<HView, HView>>>); // TODO
static_assert(!is_tec<std::unexpected<H*>>); // TODO
#endif

void test_robust_against_adl(H *h) {
  // Make sure each type can still be instantiated without error.
  [[maybe_unused]] std::array<H*, 1> t1;
  [[maybe_unused]] std::forward_list<H*>::iterator t2;
  [[maybe_unused]] std::forward_list<H*>::const_iterator t3;
  [[maybe_unused]] std::list<H*>::iterator t4;
  [[maybe_unused]] std::list<H*>::const_iterator t5;
  [[maybe_unused]] std::pair<H*, H*> t6 = std::make_pair(h, h);
  [[maybe_unused]] std::pair<H*&, H*&> t7 = std::make_pair(std::ref(h), std::ref(h));
  [[maybe_unused]] auto t8 = std::views::elements<0>(TupleView()).begin();
  [[maybe_unused]] auto t9 = std::views::iota(&h, &h).begin();
  [[maybe_unused]] std::tuple<H*> t10 = std::make_tuple(h);
  [[maybe_unused]] std::tuple<H*&> t11 = std::tie(h);
  [[maybe_unused]] std::unique_ptr<H*> t12 = nullptr;
  [[maybe_unused]] std::__wrap_iter<H**> t13;
#if TEST_STD_VER >= 23
  [[maybe_unused]] auto t14 = std::views::zip(HView(), HView()).begin();
  [[maybe_unused]] auto t15 = std::unexpected<H*>(h);
#endif
}
