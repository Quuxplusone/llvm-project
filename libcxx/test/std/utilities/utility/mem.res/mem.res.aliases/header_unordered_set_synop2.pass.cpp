//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <unordered_set>

// namespace std::pmr {
//
// typedef ... unordered_set
//
// } // namespace std::pmr

#include <unordered_set>

int main(int, char**)
{
    {
        // Check that std::pmr::unordered_set is usable without <memory_resource>.
        std::pmr::unordered_set<int> s;
        std::pmr::unordered_multiset<int> ms;
    }

  return 0;
}
