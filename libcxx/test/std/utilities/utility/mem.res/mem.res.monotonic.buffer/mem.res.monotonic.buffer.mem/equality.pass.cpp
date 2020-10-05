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
#include <new>
#include <type_traits>
#include <cassert>

struct assert_on_compare : public std::pmr::memory_resource
{
protected:
    virtual void * do_allocate(size_t, size_t)
    { assert(false); }

    virtual void do_deallocate(void *, size_t, size_t)
    { assert(false); }

    virtual bool do_is_equal(std::pmr::memory_resource const &) const noexcept
    { assert(false); }
};

int main()
{
    // Same object
    {
        std::pmr::monotonic_buffer_resource r1;
        std::pmr::monotonic_buffer_resource r2;
        assert(r1 == r1);
        assert(r1 != r2);

        std::pmr::memory_resource & p1 = r1;
        std::pmr::memory_resource & p2 = r2;
        assert(p1 == p1);
        assert(p1 != p2);
        assert(p1 == r1);
        assert(r1 == p1);
        assert(p1 != r2);
        assert(r2 != p1);
    }
    // Different types
    {
        std::pmr::monotonic_buffer_resource mono1;
        std::pmr::memory_resource & r1 = mono1;
        assert_on_compare c;
        std::pmr::memory_resource & r2 = c;
        assert(r1 != r2);
        assert(!(r1 == r2));
    }
}
