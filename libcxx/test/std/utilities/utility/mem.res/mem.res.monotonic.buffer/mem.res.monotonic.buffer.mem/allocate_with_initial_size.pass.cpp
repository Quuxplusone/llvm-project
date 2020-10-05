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

void test(size_t initial_buffer_size)
{
    globalMemCounter.reset();

    std::pmr::monotonic_buffer_resource mono1(
        initial_buffer_size,
        std::pmr::new_delete_resource()
    );
    assert(globalMemCounter.checkNewCalledEq(0));

    mono1.allocate(1, 1);
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.last_new_size >= initial_buffer_size);

    mono1.allocate(initial_buffer_size - 1, 1);
    assert(globalMemCounter.checkNewCalledEq(1));
}

int main()
{
    test(1);
    test(8);
    test(10);
    test(100);
    test(256);
    test(1000);
    test(1024);
    test(1000000);
    assert(globalMemCounter.checkOutstandingNewEq(0));
}
