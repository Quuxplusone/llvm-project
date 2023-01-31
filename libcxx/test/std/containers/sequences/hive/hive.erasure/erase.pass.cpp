//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// template <class T, class Allocator, class U>
//   typename hive<T, Allocator>::size_type
//   erase(hive<T, Allocator>& c, const U& value);

#include <hive>
#include <algorithm>
#include <optional>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"

template <class S, class U>
void test0(S s, U val, S expected, size_t expected_erased_count) {
  ASSERT_SAME_TYPE(typename S::size_type, decltype(std::erase(s, val)));
  assert(expected_erased_count == std::erase(s, val));
  assert(std::is_permutation(s.begin(), s.end(), expected.begin(), expected.end()));
}

template <class S>
void test() {
  test0(S(), 1, S(), 0);

  test0(S({1}), 1, S(), 1);
  test0(S({1}), 2, S({1}), 0);

  test0(S({1, 2}), 1, S({2}), 1);
  test0(S({1, 2}), 2, S({1}), 1);
  test0(S({1, 2}), 3, S({1, 2}), 0);
  test0(S({1, 1}), 1, S(), 2);
  test0(S({1, 1}), 3, S({1, 1}), 0);

  test0(S({1, 2, 3}), 1, S({2, 3}), 1);
  test0(S({1, 2, 3}), 2, S({1, 3}), 1);
  test0(S({1, 2, 3}), 3, S({1, 2}), 1);
  test0(S({1, 2, 3}), 4, S({1, 2, 3}), 0);

  test0(S({1, 1, 1}), 1, S(), 3);
  test0(S({1, 1, 1}), 2, S({1, 1, 1}), 0);
  test0(S({1, 1, 2}), 1, S({2}), 2);
  test0(S({1, 1, 2}), 2, S({1, 1}), 1);
  test0(S({1, 1, 2}), 3, S({1, 1, 2}), 0);
  test0(S({1, 2, 2}), 1, S({2, 2}), 1);
  test0(S({1, 2, 2}), 2, S({1}), 2);
  test0(S({1, 2, 2}), 3, S({1, 2, 2}), 0);

  //  Test cross-type erasure
  using opt = std::optional<typename S::value_type>;
  test0(S({1, 2, 1}), opt(), S({1, 2, 1}), 0);
  test0(S({1, 2, 1}), opt(1), S({2}), 2);
  test0(S({1, 2, 1}), opt(2), S({1, 1}), 1);
  test0(S({1, 2, 1}), opt(3), S({1, 2, 1}), 0);
}

struct DontMove {
  static int moves;
  int value_ = 0;
  explicit DontMove(int i) : value_(i) {}
  DontMove(const DontMove& rhs) : value_(rhs.value_) { moves += 1; }
  void operator=(const DontMove& rhs) { value_ = rhs.value_; moves += 1; }
  friend bool operator==(const DontMove& a, const DontMove& b) { return a.value_ == b.value_; }
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

  size_t count = std::erase(h, DontMove(2));
  assert(count == 10);
  assert(h.size() == 20);
  assert(DontMove::moves == 0);

  count = std::erase(h, DontMove(1));
  assert(count == 10);
  assert(h.size() == 10);
  assert(DontMove::moves == 0);

  count = std::erase(h, DontMove(3));
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
