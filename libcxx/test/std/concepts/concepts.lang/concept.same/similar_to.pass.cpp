//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts

// template<class T, class U>
// concept similar_to;

#include <concepts>
#include <type_traits>

struct S;

void CheckSimilarTo() {
  static_assert(std::similar_to<int, int>);
  static_assert(std::similar_to<int*, int*>);
  static_assert(std::similar_to<int[], int[]>);
  static_assert(std::similar_to<int[9], int[]>);
  static_assert(std::similar_to<int[], int[9]>);
  static_assert(std::similar_to<int[9], int[9]>);

  static_assert(std::similar_to<const int, int>);
  static_assert(std::similar_to<int const*, int*>);
  static_assert(std::similar_to<int *const, int*>);
  static_assert(std::similar_to<int const[], int[]>);
  static_assert(std::similar_to<int const[9], int[]>);
  static_assert(std::similar_to<int const[], int[9]>);
  static_assert(std::similar_to<int const[9], int[9]>);

  static_assert(std::similar_to<int, const int>);
  static_assert(std::similar_to<int*, int const*>);
  static_assert(std::similar_to<int*, int *const>);
  static_assert(std::similar_to<int[], int const[]>);
  static_assert(std::similar_to<int[9], int const[]>);
  static_assert(std::similar_to<int[], int const[9]>);
  static_assert(std::similar_to<int[9], int const[9]>);

  static_assert(!std::similar_to<int&, int>);
  static_assert(!std::similar_to<int, int&>);
  static_assert(!std::similar_to<int&&, int>);
  static_assert(!std::similar_to<int, int&&>);
  static_assert(!std::similar_to<int&&, int&>);
  static_assert(!std::similar_to<int&, int&&>);

  static_assert(std::similar_to<int S::*, int S::*>);
  static_assert(std::similar_to<int& (S::*)(), int& (S::*)()>);
  static_assert(std::similar_to<int& (S::*)(int*), int& (S::*)(int*)>);

  static_assert(std::similar_to<int S::*, int S::*const>);
  static_assert(std::similar_to<int& (S::*)(), int& (S::*const)()>);
  static_assert(std::similar_to<int& (S::*)(int*), int& (S::*const)(int*)>);

  static_assert(!std::similar_to<int S::*, char S::*>);
  static_assert(!std::similar_to<int& (S::*)(), const int& (S::*)()>);
  static_assert(!std::similar_to<int& (S::*)(), int (S::*)()>);
  static_assert(!std::similar_to<int& (S::*)(), int (S::*)()>);
  static_assert(!std::similar_to<int (S::*)(char), int (S::*)(int)>);
  static_assert(!std::similar_to<int& (S::*)(int*), int& (S::*)(int const*)>);
}

void CheckFunctionTypes() {
  static_assert(std::similar_to<int(), int()>);
  static_assert(std::similar_to<int(int), int(int)>);
  static_assert(std::similar_to<int(*)(int), int(*const)(int)>);

  static_assert(!std::similar_to<int(), int(int)>);
  static_assert(!std::similar_to<int(), void()>);
  static_assert(!std::similar_to<int(), int(*)()>);
  static_assert(!std::similar_to<int(int*), int(int const*)>);
  static_assert(!std::similar_to<int*(), int const*()>);
}

void CheckArraysofUnknownBound() {
  static_assert(std::similar_to<int[], int[9]>);
  static_assert(std::similar_to<int[9], int[]>);
  static_assert(std::similar_to<int(*)[], int(*)[9]>);
  static_assert(std::similar_to<int(*)[9], int(*)[]>);
  static_assert(std::similar_to<int(*)[], int(*const)[9]>);
  static_assert(std::similar_to<int(*)[9], int(*const)[]>);
  static_assert(std::similar_to<int(*const*)[], int(**)[9]>);
  static_assert(std::similar_to<int(*const*)[9], int(**)[]>);
  static_assert(std::similar_to<int *(*const*)[], int *const(**)[9]>);
  static_assert(std::similar_to<int *(*const*)[9], int *const(**)[]>);

  static_assert(!std::similar_to<int[9], int[10]>);
  static_assert(!std::similar_to<int(*)[9], int(*)[10]>);
  static_assert(!std::similar_to<int *(*const*)[9], int *const(**)[10]>);
}


template <class T, class U> requires std::same_as<T, U>
void SubsumptionTest1();

template <class T, class U> requires std::similar_to<U, T>
int SubsumptionTest1();

static_assert(std::same_as<void, decltype(SubsumptionTest1<int, int>())>);
static_assert(std::same_as<int, decltype(SubsumptionTest1<int, const int>())>);


template <class T, class U> requires std::similar_to<T, U>
void SubsumptionTest2();

template <class T, class U> requires std::similar_to<U, T> && (sizeof(T) > 0)
int SubsumptionTest2();

static_assert(std::same_as<int, decltype(SubsumptionTest2<int, int>())>);
static_assert(std::same_as<int, decltype(SubsumptionTest2<int, const int>())>);


int main(int, char**) {
  CheckSimilarTo();
  return 0;
}
