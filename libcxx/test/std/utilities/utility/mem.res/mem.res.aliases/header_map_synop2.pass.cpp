//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <map>

// namespace std::pmr {
//
// typedef ... map
//
// } // namespace std::pmr

#include <map>

int main(int, char**)
{
    {
        // Check that std::pmr::map is usable without <memory_resource>.
        std::pmr::map<int, int> m;
        std::pmr::multimap<int, int> mm;
    }

  return 0;
}
