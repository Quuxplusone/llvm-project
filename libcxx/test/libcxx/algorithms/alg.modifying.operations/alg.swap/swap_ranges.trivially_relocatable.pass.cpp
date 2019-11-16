//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03
//   because unique_ptr isn't trivially relocatable in C++03

// <algorithm>

// template<ForwardIterator Iter1, ForwardIterator Iter2>
//   requires HasSwap<Iter1::reference, Iter2::reference>
//   Iter2
//   swap_ranges(Iter1 first1, Iter1 last1, Iter2 first2);

#include <algorithm>
#include <cassert>
#include <deque>
#include <memory>
#include <type_traits>
#include <vector>

#include "test_macros.h"
#include "test_iterators.h"

int swaps = 0;

struct TR {
  std::unique_ptr<int> p_;
  friend void swap(TR& a, TR& b) {
    swaps += 1;
    a.p_.swap(b.p_);
  }
};
static_assert(std::is_trivially_relocatable<TR>::value, "");

struct NotTR {
  std::unique_ptr<int> p_;
  explicit NotTR() = default;
  NotTR(NotTR&&) = default;
  NotTR& operator=(NotTR&&) = default;
  ~NotTR() {}
  friend void swap(NotTR& a, NotTR& b) {
    swaps += 1;
    a.p_.swap(b.p_);
  }
};
static_assert(!std::is_trivially_relocatable<NotTR>::value, "");

template<class T>
std::unique_ptr<T> make_unique(T i) {
    return std::unique_ptr<T>(new T(i));
}

template<class C1, class C2>
void test(int expected_swaps)
{
  C1 a(3);
  C2 b(3);
  a[0].p_ = make_unique<int>(1);
  a[1].p_ = make_unique<int>(2);
  a[2].p_ = make_unique<int>(3);
  b[0].p_ = make_unique<int>(4);
  b[1].p_ = make_unique<int>(5);
  b[2].p_ = make_unique<int>(6);

  swaps = 0;
  std::swap_ranges(a.begin(), a.end(), b.begin());
  assert(swaps == expected_swaps);
  assert(*a[0].p_ == 4);
  assert(*a[1].p_ == 5);
  assert(*a[2].p_ == 6);
  assert(*b[0].p_ == 1);
  assert(*b[1].p_ == 2);
  assert(*b[2].p_ == 3);
}

int main(int, char**)
{
  test<std::deque<TR>, std::deque<TR> >(3);
  test<std::deque<TR>, std::vector<TR> >(3);
  test<std::vector<TR>, std::deque<TR> >(3);
  test<std::vector<TR>, std::vector<TR> >(0);
  test<std::vector<NotTR>, std::vector<NotTR> >(3);

  return 0;
}
