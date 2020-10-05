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
    {
        char buffer[100];
        std::pmr::monotonic_buffer_resource mono1((void *)buffer, 0, std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));

    globalMemCounter.reset();
    {
        std::pmr::monotonic_buffer_resource mono1(nullptr, 0, std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));
}
