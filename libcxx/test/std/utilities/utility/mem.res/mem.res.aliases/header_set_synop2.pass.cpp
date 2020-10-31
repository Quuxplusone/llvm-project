//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <set>

// namespace std::pmr {
//
// typedef ... set
//
// } // namespace std::pmr

#include <set>

int main(int, char**)
{
    {
        // Check that std::pmr::set is usable without <memory_resource>.
        std::pmr::set<int> s;
        std::pmr::multiset<int> ms;
    }

  return 0;
}
