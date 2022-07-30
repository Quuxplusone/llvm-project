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

static_assert(!std::is_constructible_v<std::move_only_function<int()>, std::in_place_type_t<int>>);
static_assert(!std::is_constructible_v<std::move_only_function<int()>, const std::in_place_type_t<int>&>);
static_assert(!std::is_constructible_v<std::move_only_function<int()>, int>);
static_assert(!std::is_constructible_v<std::move_only_function<int()>, const int&>);
static_assert(!std::is_constructible_v<std::move_only_function<int()>, void(&)()>);
static_assert(!std::is_constructible_v<std::move_only_function<int()>, void(*)()>);

static_assert(std::is_constructible_v<std::move_only_function<void()>, int(&)()>);
static_assert(std::is_constructible_v<std::move_only_function<void()>, int(*)()>);
