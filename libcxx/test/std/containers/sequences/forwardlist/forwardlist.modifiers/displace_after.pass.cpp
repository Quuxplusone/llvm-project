//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <forward_list>

// iterator erase_after(const_iterator p); // constexpr since C++26

#include <forward_list>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "MoveOnly.h"
#include "min_allocator.h"

template<class T>
TEST_CONSTEXPR_CXX26 bool test() {
  {
    typedef std::forward_list<T> C;
    T t[] = {0, 1, 2, 3, 4};
    C c(std::make_move_iterator(std::begin(t)), std::make_move_iterator(std::end(t)));

    T v = c.displace_after(std::next(c.cbefore_begin(), 4));
    assert(v == T(4));
    assert(std::distance(c.begin(), c.end()) == 4);
    assert(*std::next(c.begin(), 0) == 0);
    assert(*std::next(c.begin(), 1) == 1);
    assert(*std::next(c.begin(), 2) == 2);
    assert(*std::next(c.begin(), 3) == 3);

    v = c.displace_after(std::next(c.cbefore_begin(), 0));
    assert(v == T(0));
    assert(std::distance(c.begin(), c.end()) == 3);
    assert(*std::next(c.begin(), 0) == 1);
    assert(*std::next(c.begin(), 1) == 2);
    assert(*std::next(c.begin(), 2) == 3);

    v = c.displace_after(std::next(c.cbefore_begin(), 1));
    assert(v == T(2));
    assert(std::distance(c.begin(), c.end()) == 2);
    assert(*std::next(c.begin(), 0) == 1);
    assert(*std::next(c.begin(), 1) == 3);

    v = c.displace_after(std::next(c.cbefore_begin(), 1));
    assert(v == T(3));
    assert(std::distance(c.begin(), c.end()) == 1);
    assert(*std::next(c.begin(), 0) == 1);

    v = c.displace_after(std::next(c.cbefore_begin(), 0));
    assert(v == T(1));
    assert(std::distance(c.begin(), c.end()) == 0);
  }
#if TEST_STD_VER >= 11
  {
    typedef std::forward_list<T, min_allocator<T>> C;
    T t[] = {0, 1, 2, 3, 4};
    C c(std::make_move_iterator(std::begin(t)), std::make_move_iterator(std::end(t)));

    T v = c.displace_after(std::next(c.cbefore_begin(), 4));
    assert(v == T(4));
    assert(std::distance(c.begin(), c.end()) == 4);
    assert(*std::next(c.begin(), 0) == 0);
    assert(*std::next(c.begin(), 1) == 1);
    assert(*std::next(c.begin(), 2) == 2);
    assert(*std::next(c.begin(), 3) == 3);

    v = c.displace_after(std::next(c.cbefore_begin(), 0));
    assert(v == T(0));
    assert(std::distance(c.begin(), c.end()) == 3);
    assert(*std::next(c.begin(), 0) == 1);
    assert(*std::next(c.begin(), 1) == 2);
    assert(*std::next(c.begin(), 2) == 3);

    v = c.displace_after(std::next(c.cbefore_begin(), 1));
    assert(v == T(2));
    assert(std::distance(c.begin(), c.end()) == 2);
    assert(*std::next(c.begin(), 0) == 1);
    assert(*std::next(c.begin(), 1) == 3);

    v = c.displace_after(std::next(c.cbefore_begin(), 1));
    assert(v == T(3));
    assert(std::distance(c.begin(), c.end()) == 1);
    assert(*std::next(c.begin(), 0) == 1);

    v = c.displace_after(std::next(c.cbefore_begin(), 0));
    assert(v == T(1));
    assert(std::distance(c.begin(), c.end()) == 0);
  }
#endif

  return true;
}

int main(int, char**) {
  assert(test<int>());
  assert(test<MoveOnly>());
#if TEST_STD_VER >= 26
  static_assert(test<int>());
  static_assert(test<MoveOnly>());
#endif

  return 0;
}
