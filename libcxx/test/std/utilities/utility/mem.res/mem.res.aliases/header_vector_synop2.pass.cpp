//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <vector>

// namespace std::pmr {
//
// typedef ... vector
//
// } // namespace std::pmr

#include <vector>

int main(int, char**)
{
    {
        // Check that std::pmr::vector is usable without <memory_resource>.
        std::pmr::vector<int> l;
    }

  return 0;
}
