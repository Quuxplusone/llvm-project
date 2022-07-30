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
#include <utility>

#include "test_macros.h"
#include "../common.h"

struct S {
  void func() {}
};

static_assert(std::is_invocable_v<std::move_only_function<void()>&>);
static_assert(std::is_invocable_v<std::move_only_function<void()>>);
static_assert(std::is_invocable_v<std::move_only_function<void()>&&>);
static_assert(!std::is_invocable_v<std::move_only_function<void()> const&>);
static_assert(!std::is_invocable_v<std::move_only_function<void()> const >);
static_assert(!std::is_invocable_v<std::move_only_function<void()> const&&>);

void test() {
  {
    called                            = false;
    std::move_only_function<void()> f = &call_func;
    f();
    assert(called);
  }
  {
    called                            = false;
    std::move_only_function<void()> f = TriviallyDestructible{};
    f();
    assert(called);
  }
  {
    called                            = false;
    std::move_only_function<void()> f = TriviallyDestructibleSqueezeFit{};
    f();
    assert(called);
  }
  {
    called                            = false;
    std::move_only_function<void()> f = TriviallyDestructibleTooLarge{};
    f();
    assert(called);
  }
#ifdef TEST_COMPILER_CLANG
  {
    called                            = false;
    std::move_only_function<void()> f = TriviallyRelocatable{};
    f();
    assert(called);
  }
  {
    called                            = false;
    std::move_only_function<void()> f = TriviallyRelocatableSqueezeFit{};
    f();
    assert(called);
  }
  {
    called                            = false;
    std::move_only_function<void()> f = TriviallyRelocatableTooLarge{};
    f();
    assert(called);
  }
#endif
  {
    called                            = false;
    std::move_only_function<void()> f = NonTrivial{};
    f();
    assert(called);
  }
  {
    std::move_only_function<void(S)> f = &S::func;
    assert(f);
  }
  {
    decltype(&S::func) ptr             = nullptr;
    std::move_only_function<void(S)> f = ptr;
    assert(!f);
  }
  {
    CallType type;
    std::move_only_function<void()> f = CallTypeChecker{&type};
    f();
    assert(type == CallType::LValue);
    type = CallType::None;
    std::move(f)();
    assert(type == CallType::LValue);
  }
  {
    // Test that an int-returning function can be cast to void().
    bool called_it = false;
    auto lam = [&]() -> int { called_it = true; return 42; };
    std::move_only_function<void()> f = lam;
    f();
    assert(called_it == true);
  }
}

int main(int, char**) {
  test();

  return 0;
}
