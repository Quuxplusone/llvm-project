//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// Make sure that we don't blow up the template instantiation recursion depth
// for tuples of size <= 1024.

#include <tuple>
#include <cassert>
#include <utility>

#include "test_macros.h"

template <std::size_t... I>
constexpr bool CreateTuple(std::index_sequence<I...>) {
  std::tuple<decltype(I)...> tuple(I...);
  assert(std::get<0>(tuple) == 0);
  assert(std::get<sizeof...(I)-1>(tuple) == sizeof...(I)-1);
  return true;
}

template <std::size_t... I>
TEST_CONSTEXPR_CXX20 bool AssignTuple(std::index_sequence<I...>) {
  std::tuple<decltype(I)...> tuple(I...);
  auto t2 = tuple;             // copy constructor
  auto t3 = std::move(tuple);  // move constructor
  decltype(tuple) t4;          // default constructor
  t3 = t2;                     // copy assignment
  t4 = std::move(t2);          // move assignment
  swap(t3, t4);                // swap
  // equality (t3 == t4) does not work
  return true;
}

int main(int, char**) {
  CreateTuple(std::make_index_sequence<1024>{});
  static_assert(CreateTuple(std::make_index_sequence<1024>{}), "");
  AssignTuple(std::make_index_sequence<1024>{});
#if TEST_STD_VER >= 20
  static_assert(AssignTuple(std::make_index_sequence<1024>{}));
#endif

  return 0;
}
