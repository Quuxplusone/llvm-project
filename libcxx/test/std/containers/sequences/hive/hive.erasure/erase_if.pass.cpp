//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// template <class T, class Allocator, class Predicate>
//   typename hive<T, Allocator>::size_type
//   erase_if(hive<T, Allocator>& c, Predicate pred);

#include <hive>
#include <algorithm>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"

template <class S, class Pred>
void test0(S s, Pred p, S expected, size_t expected_erased_count) {
  ASSERT_SAME_TYPE(typename S::size_type, decltype(std::erase_if(s, p)));
  assert(expected_erased_count == std::erase_if(s, p));
  assert(std::is_permutation(s.begin(), s.end(), expected.begin(), expected.end()));
}

template <class S>
void test() {
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

struct DontMove {
  static int moves;
  int value_ = 0;
  explicit DontMove(int i) : value_(i) {}
  DontMove(const DontMove& rhs) : value_(rhs.value_) { moves += 1; }
  void operator=(const DontMove& rhs) { value_ = rhs.value_; moves += 1; }
};
int DontMove::moves = 0;

void test_erase_doesnt_move_elements() {
  std::hive<DontMove> h;
  for (int i=0; i < 10; ++i) {
    h.emplace(1);
    h.emplace(2);
    h.emplace(3);
  }
  assert(h.size() == 30);

  size_t count = std::erase_if(h, [](auto& x) { return x.value_ == 2; });
  assert(count == 10);
  assert(h.size() == 20);
  assert(DontMove::moves == 0);

  count = std::erase_if(h, [](auto& x) { return x.value_ == 3; });
  assert(count == 10);
  assert(h.size() == 10);
  assert(DontMove::moves == 0);

  count = std::erase_if(h, [](auto& x) { return x.value_ == 1; });
  assert(count == 10);
  assert(h.empty());
  assert(DontMove::moves == 0);
}

int main(int, char**) {
  test<std::hive<int>>();
  test<std::hive<int, min_allocator<int>>>();
  test<std::hive<int, test_allocator<int>>>();
  test<std::hive<long>>();
  test<std::hive<double>>();
  test_erase_doesnt_move_elements();

  return 0;
}
