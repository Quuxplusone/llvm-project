//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <deque>

// namespace std::pmr {
//
// typedef ... deque
//
// } // namespace std::pmr

#include <deque>

int main(int, char**)
{
    {
        // Check that std::pmr::deque is usable without <memory_resource>.
        std::pmr::deque<int> d;
    }

  return 0;
}
