//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <list>

// namespace std::pmr {
//
// typedef ... list
//
// } // namespace std::pmr

#include <list>

int main(int, char**)
{
    {
        // Check that std::pmr::list is usable without <memory_resource>.
        std::pmr::list<int> l;
    }

  return 0;
}
