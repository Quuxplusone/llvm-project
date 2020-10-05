//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <memory_resource>

// monotonic_buffer_resource(monotonic_buffer_resource const&) = delete;
// monotonic_buffer_resource& operator=(monotonic_buffer_resource const&) = delete;

#include <memory_resource>
#include <type_traits>
#include <cassert>

int main()
{
    using MBR = std::pmr::monotonic_buffer_resource;
    static_assert(!std::is_copy_constructible<MBR>::value, "");
    static_assert(!std::is_move_constructible<MBR>::value, "");
    static_assert(!std::is_copy_assignable<MBR>::value, "");
    static_assert(!std::is_move_assignable<MBR>::value, "");
}
