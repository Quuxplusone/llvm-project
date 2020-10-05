//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <memory_resource>

// template <class T> class polymorphic_allocator

// T* polymorphic_allocator<T>::deallocate(T*, size_t size)

int AssertCount = 0;

#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : (void)::AssertCount++)
#define _LIBCPP_DEBUG 0
#include <memory_resource>
#include <type_traits>
#include <cassert>

#include "test_std_memory_resource.h"

int main(int, char**)
{
    using Alloc = std::pmr::polymorphic_allocator<int>;
    using Traits = std::allocator_traits<Alloc>;
    NullResource R;
    Alloc a(&R);
    const std::size_t maxSize = Traits::max_size(a);

    a.deallocate(nullptr, maxSize);
    assert(AssertCount == 0);
    a.deallocate(nullptr, maxSize + 1);
    assert(AssertCount == 1);

    return 0;
}
