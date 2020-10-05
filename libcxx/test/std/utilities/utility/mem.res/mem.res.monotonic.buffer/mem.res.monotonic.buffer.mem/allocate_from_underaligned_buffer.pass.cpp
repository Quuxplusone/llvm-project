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
        alignas(4) char buffer[17];
        std::pmr::monotonic_buffer_resource mono1(buffer + 1, 16, std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret == buffer + 1);
        mono1.release();

        ret = r1.allocate(1, 2);
        assert(ret == buffer + 2);
        mono1.release();

        ret = r1.allocate(1, 4);
        assert(ret == buffer + 4);
        mono1.release();

        // Test a size that is just big enough to fit in the buffer,
        // but can't fit if it's aligned.
        ret = r1.allocate(16, 1);
        assert(ret == buffer + 1);
        mono1.release();

        assert(globalMemCounter.checkNewCalledEq(0));
        ret = r1.allocate(16, 2);
        assert(globalMemCounter.checkNewCalledEq(1));
        assert(globalMemCounter.last_new_size >= 16);
        // assert(globalMemCounter.last_new_align >= 2);
        mono1.release();
        assert(globalMemCounter.checkDeleteCalledEq(1));
        // assert(globalMemCounter.last_new_align == globalMemCounter.last_delete_align);
    }
}
