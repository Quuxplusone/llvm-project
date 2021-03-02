//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <queue>

// template <class InputIterator>
//   priority_queue(InputIterator first, InputIterator last, const Compare& comp);

#include <queue>
#include <vector>
#include <cassert>

#include "test_macros.h"

template<class Seq>
std::true_type test(int, decltype(Seq(1,2))) {
    return {};
}

template<class Seq>
std::false_type test(long, Seq) {
    return {};
}

int main(int, char**)
{
    // Sanity-check that std::vector<int>(1,2) is well-formed.
    auto vector_is_sequence = test< std::vector<int> >(1, {});
    static_assert(decltype(vector_is_sequence)::value, "");

    // LWG3522: std::priority_queue<int>(1,2) is NOT well-formed.
    auto pq_is_sequence = test< std::priority_queue<int> >(1, {});
    static_assert(!decltype(pq_is_sequence)::value, "");

    return 0;
}
