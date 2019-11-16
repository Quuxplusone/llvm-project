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

struct Cxx20Friendly {
  int head_;
  std::string strings_[100];
  int tail_;
};

struct Cxx23Friendly {
  int head_;
  std::unique_ptr<int> ptrs_[1000];
  int tail_;
};

TEST_CONSTEXPR_CXX20 bool test_swap_strings()
{
  Cxx20Friendly a = { 1, {}, 2 };
  Cxx20Friendly b = { 3, {}, 4 };
  a.strings_[1] = "1";
  a.strings_[10] = "100";
  a.strings_[99] = "999";
  b.strings_[1] = "420";
  b.strings_[60] = "600";
  std::swap(a, b);
  assert(a.head_ == 3);
  assert(a.strings_[1] == "420");
  assert(a.strings_[10] == "");
  assert(a.strings_[60] == "600");
  assert(a.strings_[99] == "");
  assert(a.tail_ == 4);
  assert(b.head_ == 1);
  assert(b.strings_[1] == "1");
  assert(b.strings_[10] == "100");
  assert(b.strings_[60] == "");
  assert(b.strings_[99] == "999");
  assert(b.tail_ == 2);
  return true;
}

TEST_CONSTEXPR_CXX23 bool test_swap_unique_ptrs()
{
  Cxx23Friendly a = { 1, {}, 2 };
  Cxx23Friendly b = { 3, {}, 4 };
  a.ptrs_[42] = std::unique_ptr<int>(new int(42));
  a.ptrs_[100] = std::unique_ptr<int>(new int(100));
  b.ptrs_[42] = std::unique_ptr<int>(new int(420));
  b.ptrs_[200] = std::unique_ptr<int>(new int(1000));
  b.ptrs_[999] = std::unique_ptr<int>(new int(9990));
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
  return true;
}

int main(int, char**)
{
  test_swap_strings();
  test_swap_unique_ptrs();
#if TEST_STD_VER >= 20
  static_assert(test_swap_strings());
#endif // TEST_STD_VER >= 20
#if TEST_STD_VER >= 23
  static_assert(test_swap_unique_ptrs());
#endif // TEST_STD_VER >= 23
  return 0;
}
