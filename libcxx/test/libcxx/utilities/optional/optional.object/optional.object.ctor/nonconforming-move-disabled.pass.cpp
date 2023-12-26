//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <optional>

// optional(optional<T>&& rhs);
// Verify that the move constructor uses relocation for types that are trivially relocatable
// but not trivially copyable, when _LIBCPP_NONCONFORMING_OPTIONAL_MOVE_CTOR is disabled.

#include <cassert>
#include <optional>
#include <memory>
#include <utility>

#include "test_macros.h"

int main(int, char**)
{
  {
    std::optional<int> i = 42;
    std::optional<int> j = std::move(i); // i remains engaged
    assert(i.has_value());
    assert(i.value() == 42);
    assert(j.has_value());
    assert(j.value() == 42);
  }
  {
    std::optional<std::shared_ptr<int>> i = std::make_shared<int>(42);
    std::optional<std::shared_ptr<int>> j = std::move(i); // i remains engaged
    assert(i.has_value());
    assert(i.value() == nullptr);
    assert(j.has_value());
    assert(j.value() != nullptr && *j.value() == 42);
  }
  {
    std::optional<std::shared_ptr<int>> i = std::make_shared<int>(42);
    std::optional<std::shared_ptr<void>> j = std::move(i); // i remains engaged
    assert(i.has_value());
    assert(i.value() == nullptr);
    assert(j.has_value());
    assert(j.value() != nullptr && *(int*)j.value().get() == 42);
  }
  {
    std::optional<std::unique_ptr<int>> i = std::make_unique<int>(42);
    std::optional<std::shared_ptr<int>> j = std::move(i); // i remains engaged
    assert(i.has_value());
    assert(i.value() == nullptr);
    assert(j.has_value());
    assert(j.value() != nullptr && *j.value() == 42);
  }
  {
    std::optional<std::unique_ptr<int>> i = std::make_unique<int>(42);
    std::optional<std::unique_ptr<int>> j = std::move(i); // i remains engaged
    assert(i.has_value());
    assert(i.value() == nullptr);
    assert(j.has_value());
    assert(j.value() != nullptr && *j.value() == 42);
  }

  return 0;
}
