//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

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
