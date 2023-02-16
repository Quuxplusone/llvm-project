//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <utility>

// template<class T>
//   void swap(T& a, T& b);

// Verify the behavior of swapping very large trivially relocatable objects.

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "test_macros.h"

struct Large {
    int head_;
    std::unique_ptr<int> ptrs_[1000];
    int tail_;
};

#if TEST_STD_VER >= 20
constexpr bool test_swap_constexpr()
{
  Large a = { 1, {}, 2 };
  Large b = { 3, {}, 4 };
  std::swap(a, b);
  assert(a.head_ == 3);
  assert(a.tail_ == 4);
  assert(b.head_ == 1);
  assert(b.tail_ == 2);
  return true;
}
static_assert(test_swap_constexpr());
#endif // TEST_STD_VER >= 20

int main(int, char**)
{
  {
    Large a = { 1, {}, 2 };
    Large b = { 3, {}, 4 };
    a.ptrs_[42] = std::make_unique<int>(42);
    a.ptrs_[100] = std::make_unique<int>(100);
    b.ptrs_[42] = std::make_unique<int>(420);
    b.ptrs_[200] = std::make_unique<int>(1000);
    b.ptrs_[999] = std::make_unique<int>(9990);
    std::swap(a, b);
    assert(a.head_ == 3);
    assert(a.ptrs_[42] != nullptr && *a.ptrs_[42] == 420);
    assert(a.ptrs_[100] == nullptr);
    assert(a.ptrs_[200] != nullptr && *a.ptrs_[200] == 1000);
    assert(a.ptrs_[999] != nullptr && *a.ptrs_[999] == 9990);
    assert(a.tail_ == 4);
    assert(b.head_ == 1);
    assert(b.ptrs_[42] != nullptr && *b.ptrs_[42] == 42);
    assert(b.ptrs_[100] != nullptr && *b.ptrs_[100] == 100);
    assert(b.ptrs_[200] == nullptr);
    assert(b.ptrs_[999] == nullptr);
    assert(b.tail_ == 2);
  }
  return 0;
}
