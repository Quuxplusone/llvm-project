//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <queue>
#include <list>

#include "../../from_range_container_adaptors.h"
#include "test_macros.h"

// template<container-compatible-range<T> R> queue(from_range_t, R&& rg); // since C++23
// template<container-compatible-range<T> R, class Alloc>
//   queue(from_range_t, R&& rg, const Alloc&); // since C++23

template<class T, class C = std::list<T>>
using std_queue = std::queue<T, C>;

TEST_CONSTEXPR_CXX26 bool test() {
  for_all_iterators_and_allocators<int>([]<class Iter, class Sent, class Alloc>() {
    test_container_adaptor<std_queue, std::list, int, Iter, Sent, Alloc>();
  });
  test_container_adaptor_move_only<std_queue>();

  static_assert(test_constraints<std::queue, int, double>());

  if (!std::is_constant_evaluated()) {
    test_exception_safety_throwing_copy<std::queue>();
    test_exception_safety_throwing_allocator<std::queue, int>();
  }

  return true;
}

int main(int, char**) {
  test();
#if TEST_STD_VER >= 26
  static_assert(test());
#endif

  return 0;
}
