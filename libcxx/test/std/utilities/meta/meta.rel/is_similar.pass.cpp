//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// is_same

#include <type_traits>

struct S;

void CheckIsSimilar() {
  static_assert(std::is_similar<int, int>::value, "");
  static_assert(std::is_similar<int*, int*>::value, "");
  static_assert(std::is_similar<int[], int[]>::value, "");
  static_assert(std::is_similar<int[9], int[]>::value, "");
  static_assert(std::is_similar<int[], int[9]>::value, "");
  static_assert(std::is_similar<int[9], int[9]>::value, "");

  static_assert(std::is_similar<const int, int>::value, "");
  static_assert(std::is_similar<int const*, int*>::value, "");
  static_assert(std::is_similar<int *const, int*>::value, "");
  static_assert(std::is_similar<int const[], int[]>::value, "");
  static_assert(std::is_similar<int const[9], int[]>::value, "");
  static_assert(std::is_similar<int const[], int[9]>::value, "");
  static_assert(std::is_similar<int const[9], int[9]>::value, "");

  static_assert(std::is_similar<int, const int>::value, "");
  static_assert(std::is_similar<int*, int const*>::value, "");
  static_assert(std::is_similar<int*, int *const>::value, "");
  static_assert(std::is_similar<int[], int const[]>::value, "");
  static_assert(std::is_similar<int[9], int const[]>::value, "");
  static_assert(std::is_similar<int[], int const[9]>::value, "");
  static_assert(std::is_similar<int[9], int const[9]>::value, "");

  static_assert(!std::is_similar<int&, int>::value, "");
  static_assert(!std::is_similar<int, int&>::value, "");
  static_assert(!std::is_similar<int&&, int>::value, "");
  static_assert(!std::is_similar<int, int&&>::value, "");
  static_assert(!std::is_similar<int&&, int&>::value, "");
  static_assert(!std::is_similar<int&, int&&>::value, "");

  static_assert(std::is_similar<int S::*, int S::*>::value, "");
  static_assert(std::is_similar<int& (S::*)(), int& (S::*)()>::value, "");
  static_assert(std::is_similar<int& (S::*)(int*), int& (S::*)(int*)>::value, "");

  static_assert(std::is_similar<int S::*, int S::*const>::value, "");
  static_assert(std::is_similar<int& (S::*)(), int& (S::*const)()>::value, "");
  static_assert(std::is_similar<int& (S::*)(int*), int& (S::*const)(int*)>::value, "");

  static_assert(!std::is_similar<int S::*, char S::*>::value, "");
  static_assert(!std::is_similar<int& (S::*)(), const int& (S::*)()>::value, "");
  static_assert(!std::is_similar<int& (S::*)(), int (S::*)()>::value, "");
  static_assert(!std::is_similar<int& (S::*)(), int (S::*)()>::value, "");
  static_assert(!std::is_similar<int (S::*)(char), int (S::*)(int)>::value, "");
  static_assert(!std::is_similar<int& (S::*)(int*), int& (S::*)(int const*)>::value, "");
}

int main(int, char**) {
  CheckIsSimilar();
  return 0;
}
