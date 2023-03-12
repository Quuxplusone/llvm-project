//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// void displace_back();

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "MoveOnly.h"


TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        std::vector<int> c;
        c.push_back(1);
        assert(c.size() == 1);
        int x = c.displace_back();
        assert(x == 1);
        assert(c.size() == 0);

    }
#if TEST_STD_VER >= 11
    {
        std::vector<int, min_allocator<int>> c;
        c.push_back(1);
        assert(c.size() == 1);
        int x = c.displace_back();
        assert(x == 1);
        assert(c.size() == 0);
    }
    {
        std::vector<MoveOnly, min_allocator<MoveOnly>> c;
        c.push_back(1);
        assert(c.size() == 1);
        MoveOnly x = c.displace_back();
        assert(x == 1);
        assert(c.size() == 0);
    }
#endif
    return true;
}

int main(int, char**)
{
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
