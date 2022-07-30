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

int main(int, char**) {
  std::move_only_function<int() const noexcept> disengaged = nullptr;
  assert(!disengaged);
  {
    std::move_only_function<int() const noexcept> f = std::move(disengaged);
    assert(!f);
  }
  {
    std::move_only_function<int() const> f = std::move(disengaged);
    assert(!f);
  }
  {
    std::move_only_function<int() noexcept> f = std::move(disengaged);
    assert(!f);
  }
  {
    std::move_only_function<int()> f = std::move(disengaged);
    assert(!f);
  }
  {
    std::move_only_function<void() const noexcept> f = std::move(disengaged);
    assert(!f);
  }
  {
    std::move_only_function<void() const> f = std::move(disengaged);
    assert(!f);
  }
  {
    std::move_only_function<void() noexcept> f = std::move(disengaged);
    assert(!f);
  }
  {
    std::move_only_function<void()> f = std::move(disengaged);
    assert(!f);
  }
  return 0;
}
