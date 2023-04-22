//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<ForwardIterator Iter, BinaryPredicate pred>
//   constexpr bool is_uniqued(Iter first, Iter last, BinaryPredicate pred);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

TEST_CONSTEXPR_CXX20 bool less(int a, int b) {
  return a < b;
}

TEST_CONSTEXPR_CXX20 bool test_constexpr() {
  int ia[] = {9, 8, 7, 6, 5, 6, 3, 2};
  int ib[] = {9, 8, 7, 6, 5, 4, 3, 2};
  assert(std::is_uniqued(ia, ia + 8, less) == false);
  assert(std::is_uniqued(ib, ib + 8, less) == true);
  return true;
}

bool equal_mod3(int a, int b) {
  return (a % 3) == (b % 3);
}

int main(int, char**)
{
  {
    int ia[] = {0, 1, 2, 5, 0, 1, 2, 3};
    assert(!std::is_uniqued(ia, ia + 8, equal_mod3));
    assert(std::is_uniqued(ia, ia + 3, equal_mod3));
    assert(!std::is_uniqued(ia, ia + 4, equal_mod3));
    assert(std::is_uniqued(ia, ia, equal_mod3));
    assert(!std::is_uniqued(ia + 2, ia + 8, equal_mod3));
    assert(std::is_uniqued(ia + 3, ia + 8, equal_mod3));
  }
  {
    using It = forward_iterator<const int*>;
    int ia[] = {0, 1, 2, 5, 0, 1, 2, 3};
    assert(!std::is_uniqued(It(ia), It(ia + 8), equal_mod3));
    assert(std::is_uniqued(It(ia), It(ia + 3), equal_mod3));
    assert(!std::is_uniqued(It(ia), It(ia + 4), equal_mod3));
    assert(std::is_uniqued(It(ia), It(ia), equal_mod3));
    assert(!std::is_uniqued(It(ia + 2), It(ia + 8), equal_mod3));
    assert(std::is_uniqued(It(ia + 3), It(ia + 8), equal_mod3));
  }

  test_constexpr();
#if TEST_STD_VER >= 20
  static_assert(test_constexpr());
#endif

  return 0;
}
