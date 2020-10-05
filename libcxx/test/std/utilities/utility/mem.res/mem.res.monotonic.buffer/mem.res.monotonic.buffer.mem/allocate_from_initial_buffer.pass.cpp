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

#include "count_new.h"

int main()
{
    globalMemCounter.reset();
    char buffer[100];
    std::pmr::monotonic_buffer_resource mono1((void *)buffer, sizeof buffer, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = mono1;

    // Check that construction with a buffer does not allocate anything from the upstream
    assert(globalMemCounter.checkNewCalledEq(0));

    // Check that an allocation that fits in the buffer does not allocate anything from the upstream
    void *ret = r1.allocate(50);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(0));

    // Check a second allocation
    ret = r1.allocate(20);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(0));

    r1.deallocate(ret, 50);
    assert(globalMemCounter.checkDeleteCalledEq(0));

    // Check an allocation that doesn't fit in the original buffer
    ret = r1.allocate(50);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(1));

    r1.deallocate(ret, 50);
    assert(globalMemCounter.checkDeleteCalledEq(0));

    mono1.release();
    assert(globalMemCounter.checkDeleteCalledEq(1));
    assert(globalMemCounter.checkOutstandingNewEq(0));
}
