//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<ForwardIterator Iter>
//   constexpr bool is_uniqued(Iter first, Iter last);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

TEST_CONSTEXPR_CXX20 bool test_constexpr() {
  int ia[] = {0, 1, 2, 2, 0, 1, 2, 3};
  int ib[] = {0, 1, 2, 7, 0, 1, 2, 3};
  assert(std::is_uniqued(ia, ia + 8) == false);
  assert(std::is_uniqued(ib, ib + 8) == true);
  return true;
}

int main(int, char**)
{
  {
    int ia[] = {0, 1, 2, 2, 0, 1, 2, 3};
    assert(!std::is_uniqued(ia, ia + 8));
    assert(std::is_uniqued(ia, ia + 3));
    assert(!std::is_uniqued(ia, ia + 4));
    assert(std::is_uniqued(ia, ia));
    assert(!std::is_uniqued(ia + 2, ia + 8));
    assert(std::is_uniqued(ia + 3, ia + 8));
  }
  {
    using It = forward_iterator<const int*>;
    int ia[] = {0, 1, 2, 2, 0, 1, 2, 3};
    assert(!std::is_uniqued(It(ia), It(ia + 8)));
    assert(std::is_uniqued(It(ia), It(ia + 3)));
    assert(!std::is_uniqued(It(ia), It(ia + 4)));
    assert(std::is_uniqued(It(ia), It(ia)));
    assert(!std::is_uniqued(It(ia + 2), It(ia + 8)));
    assert(std::is_uniqued(It(ia + 3), It(ia + 8)));
  }

  test_constexpr();
#if TEST_STD_VER >= 20
  static_assert(test_constexpr());
#endif

  return 0;
}
