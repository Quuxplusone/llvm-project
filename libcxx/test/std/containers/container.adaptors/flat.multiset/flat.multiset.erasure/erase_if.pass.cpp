//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<class Key, class Compare, class KeyContainer, class Predicate>
//   typename flat_multiset<Key, Compare, KeyContainer>::size_type
//   erase_if(flat_multiset<Key, Compare, KeyContainer>& c, Predicate pred);

#include <deque>
#include <flat_set>
#include <functional>
#include <vector>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"

// Verify that `flat_multiset` (like `multiset`) does NOT support std::erase.
//
template <class S>
concept HasStdErase = requires (S& s, typename S::value_type x) {
  std::erase(s, x);
};
static_assert(HasStdErase<std::vector<int>>);
static_assert(!HasStdErase<std::flat_multiset<int>>);

template <class S, class Pred>
void test0(S s, Pred p, S expected, size_t expected_erased_count) {
  ASSERT_SAME_TYPE(typename S::size_type, decltype(std::erase_if(s, p)));
  assert(expected_erased_count == std::erase_if(s, p));
  assert(s == expected);
}

template <class S>
void test()
{
  auto is1 = [](auto v) { return v == 1;};
  auto is2 = [](auto v) { return v == 2;};
  auto is3 = [](auto v) { return v == 3;};
  auto is4 = [](auto v) { return v == 4;};
  auto True  = [](auto) { return true; };
  auto False = [](auto) { return false; };

  test0(S(), is1, S(), 0);

  test0(S({1}), is1, S(), 1);
  test0(S({1}), is2, S({1}), 0);

  test0(S({1, 2}), is1, S({2}), 1);
  test0(S({1, 2}), is2, S({1}), 1);
  test0(S({1, 2}), is3, S({1, 2}), 0);
  test0(S({1, 1}), is1, S(), 2);
  test0(S({1, 1}), is3, S({1, 1}), 0);

  test0(S({1, 2, 3}), is1, S({2, 3}), 1);
  test0(S({1, 2, 3}), is2, S({1, 3}), 1);
  test0(S({1, 2, 3}), is3, S({1, 2}), 1);
  test0(S({1, 2, 3}), is4, S({1, 2, 3}), 0);

  test0(S({1, 1, 1}), is1, S(), 3);
  test0(S({1, 1, 1}), is2, S({1, 1, 1}), 0);
  test0(S({1, 1, 2}), is1, S({2}), 2);
  test0(S({1, 1, 2}), is2, S({1, 1}), 1);
  test0(S({1, 1, 2}), is3, S({1, 1, 2}), 0);
  test0(S({1, 2, 2}), is1, S({2, 2}), 1);
  test0(S({1, 2, 2}), is2, S({1}), 2);
  test0(S({1, 2, 2}), is3, S({1, 2, 2}), 0);

  test0(S({1, 2, 3}), True, S(), 3);
  test0(S({1, 2, 3}), False, S({1, 2, 3}), 0);
}

int main(int, char**)
{
  test<std::flat_multiset<int>>();
  test<std::flat_multiset<int, std::less<int>, std::vector<int, min_allocator<int>>>>();
  test<std::flat_multiset<int, std::greater<int>, std::vector<int, test_allocator<int>>>>();
  test<std::flat_multiset<int, std::less<int>, std::deque<int, min_allocator<int>>>>();
  test<std::flat_multiset<int, std::greater<int>, std::deque<int, test_allocator<int>>>>();
  test<std::flat_multiset<long>>();
  test<std::flat_multiset<double>>();
  {
    std::flat_multiset<bool> fs = {false, true};
    std::same_as<size_t> auto n = std::erase_if(fs, [](auto x) { return x; });
    assert(fs == std::flat_multiset<bool>{false});
    assert(n == 1);
  }
  {
    std::flat_multiset<bool> fs = {false, false, true, true, true};
    std::same_as<size_t> auto n = std::erase_if(fs, [](auto x) { return !x; });
    assert((fs == std::flat_multiset<bool>{true, true, true}));
    assert(n == 2);
  }
  return 0;
}
