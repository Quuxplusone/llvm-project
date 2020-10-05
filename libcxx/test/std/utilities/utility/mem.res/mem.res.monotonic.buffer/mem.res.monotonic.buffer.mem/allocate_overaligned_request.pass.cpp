//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <memory_resource>

// class monotonic_buffer_resource

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"

int main()
{
#ifndef TEST_HAS_NO_ALIGNED_ALLOCATION

    globalMemCounter.reset();
    std::pmr::monotonic_buffer_resource mono1(1024, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = mono1;

    constexpr size_t big_alignment = 8 * alignof(std::max_align_t);
    static_assert(big_alignment > 4);

    void *ret = r1.allocate(2048, big_alignment);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.checkAlignedNewCalledEq(1));
    assert(globalMemCounter.checkLastNewSizeGe(2048));
    assert(globalMemCounter.checkLastNewAlignEq(big_alignment));

    // Check that a single highly aligned allocation request doesn't
    // permanently "poison" the resource to allocate only super-aligned
    // blocks of memory.
    ret = r1.allocate(globalMemCounter.last_new_size, 4);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(2));
    assert(globalMemCounter.checkAlignedNewCalledEq(1));

#endif
}
