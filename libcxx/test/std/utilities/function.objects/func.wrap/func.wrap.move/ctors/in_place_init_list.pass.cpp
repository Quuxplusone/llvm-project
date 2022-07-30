//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <functional>

#include "test_macros.h"
#include "../common.h"

template <class T>
void test() {
  {
    int counter = 0;
    std::move_only_function<T> f{std::in_place_type<TriviallyDestructible>, {1}, MoveCounter{&counter}};
    assert(f);
    assert(counter == 1);
  }
  {
    int counter = 0;
    std::move_only_function<T> f{std::in_place_type<TriviallyDestructibleSqueezeFit>, {1}, MoveCounter{&counter}};
    assert(f);
    assert(counter == 1);
  }
  {
    int counter = 0;
    std::move_only_function<T> f{std::in_place_type<TriviallyDestructibleTooLarge>, {1}, MoveCounter{&counter}};
    assert(f);
    assert(counter == 1);
  }
#ifdef TEST_COMPILER_CLANG
  {
    int counter = 0;
    std::move_only_function<T> f{std::in_place_type<TriviallyRelocatable>, {1}, MoveCounter{&counter}};
    assert(f);
    assert(counter == 1);
  }
  {
    int counter = 0;
    std::move_only_function<T> f{std::in_place_type<TriviallyRelocatableSqueezeFit>, {1}, MoveCounter{&counter}};
    assert(f);
    assert(counter == 1);
  }
  {
    int counter = 0;
    std::move_only_function<T> f{std::in_place_type<TriviallyRelocatableTooLarge>, {1}, MoveCounter{&counter}};
    assert(f);
    assert(counter == 1);
  }
#endif
  {
    int counter = 0;
    std::move_only_function<T> f{std::in_place_type<NonTrivial>, {1}, MoveCounter{&counter}};
    assert(f);
    assert(counter == 1);
  }
}

int main(int, char**) {
  call_test<void()>([]<class T> { test<T>(); });
}
