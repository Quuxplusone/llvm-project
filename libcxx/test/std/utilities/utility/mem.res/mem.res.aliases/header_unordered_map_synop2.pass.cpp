//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <unordered_map>

// namespace std::pmr {
//
// typedef ... unordered_map
//
// } // namespace std::pmr

#include <unordered_map>

int main(int, char**)
{
    {
        // Check that std::pmr::unordered_map is usable without <memory_resource>.
        std::pmr::unordered_map<int, int> m;
        std::pmr::unordered_multimap<int, int> mm;
    }

  return 0;
}
