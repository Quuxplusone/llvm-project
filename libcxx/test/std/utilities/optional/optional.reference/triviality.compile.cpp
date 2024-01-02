//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20, c++23

// <optional>

// optional<T&> should be trivially copyable

#include <optional>
#include <string>
#include <type_traits>

#include "test_macros.h"

static_assert(std::is_trivially_copyable_v<std::optional<int&>>);
static_assert(std::is_trivially_copyable_v<std::optional<const int&>>);
static_assert(std::is_trivially_copyable_v<std::optional<int&&>>);
static_assert(std::is_trivially_copyable_v<std::optional<const int&&>>);

static_assert(std::is_trivially_copyable_v<std::optional<std::string&>>);
static_assert(std::is_trivially_copyable_v<std::optional<const std::string&>>);
static_assert(std::is_trivially_copyable_v<std::optional<std::string&&>>);
static_assert(std::is_trivially_copyable_v<std::optional<const std::string&&>>);

static_assert(std::is_trivially_copyable_v<std::optional<void(&)()>>);
