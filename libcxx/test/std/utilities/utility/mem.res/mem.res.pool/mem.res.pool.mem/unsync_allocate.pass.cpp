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

// class unsynchronized_pool_resource

#include <memory_resource>
#include <cassert>

#include "count_new.h"

int main()
{
    {
        globalMemCounter.reset();

        std::pmr::unsynchronized_pool_resource unsync1(std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = unsync1;

        void *ret = r1.allocate(50);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        r1.deallocate(ret, 50);
        unsync1.release();
        assert(globalMemCounter.checkDeleteCalledGreaterThan(0));
        assert(globalMemCounter.checkOutstandingNewEq(0));

        globalMemCounter.reset();

        ret = r1.allocate(500);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        // Check that the destructor calls release()
    }
    assert(globalMemCounter.checkDeleteCalledGreaterThan(0));
    assert(globalMemCounter.checkOutstandingNewEq(0));
}
