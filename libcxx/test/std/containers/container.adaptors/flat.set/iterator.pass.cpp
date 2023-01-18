//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

//       iterator begin();
// const_iterator begin() const;
//       iterator end();
// const_iterator end()   const;
//
//       reverse_iterator rbegin();
// const_reverse_iterator rbegin() const;
//       reverse_iterator rend();
// const_reverse_iterator rend()   const;
//
// const_iterator         cbegin()  const;
// const_iterator         cend()    const;
// const_reverse_iterator crbegin() const;
// const_reverse_iterator crend()   const;

#include <cassert>
#include <cstddef>
#include <deque>
#include <flat_set>
#include <functional>

#include "test_macros.h"

int main(int, char**)
{
  {
    using M = std::flat_set<int, std::less<int>, std::deque<int>>;
    M m = {1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8};
    ASSERT_SAME_TYPE(decltype(m.begin()), M::iterator);
    ASSERT_SAME_TYPE(decltype(m.cbegin()), M::const_iterator);
    ASSERT_SAME_TYPE(decltype(m.rbegin()), M::reverse_iterator);
    ASSERT_SAME_TYPE(decltype(m.crbegin()), M::const_reverse_iterator);
    ASSERT_SAME_TYPE(decltype(m.end()), M::iterator);
    ASSERT_SAME_TYPE(decltype(m.cend()), M::const_iterator);
    ASSERT_SAME_TYPE(decltype(m.rend()), M::reverse_iterator);
    ASSERT_SAME_TYPE(decltype(m.crend()), M::const_reverse_iterator);
    assert(m.size() == 8);
    assert(std::distance(m.begin(), m.end()) == 8);
    assert(std::distance(m.cbegin(), m.cend()) == 8);
    assert(std::distance(m.rbegin(), m.rend()) == 8);
    assert(std::distance(m.crbegin(), m.crend()) == 8);
    M::iterator i;  // default-construct
    i = m.begin();  // move-assignment
    M::const_iterator k = i;  // converting constructor
    assert(i == k);  // comparison
    for (int j = 1; j <= 8; ++j, ++i) {
      assert(*i == j);  // pre-increment, dereference
    }
    assert(i == m.end());
    for (int j = 8; j >= 1; --j) {
      --i;  // pre-decrement
      assert(*i == j);
    }
    assert(i == m.begin());
  }
  {
    using M = std::flat_set<short, std::less<>, std::deque<short>>;
    const M m = {1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8};
    ASSERT_SAME_TYPE(decltype(m.begin()), M::const_iterator);
    ASSERT_SAME_TYPE(decltype(m.cbegin()), M::const_iterator);
    ASSERT_SAME_TYPE(decltype(m.rbegin()), M::const_reverse_iterator);
    ASSERT_SAME_TYPE(decltype(m.crbegin()), M::const_reverse_iterator);
    ASSERT_SAME_TYPE(decltype(m.end()), M::const_iterator);
    ASSERT_SAME_TYPE(decltype(m.cend()), M::const_iterator);
    ASSERT_SAME_TYPE(decltype(m.rend()), M::const_reverse_iterator);
    ASSERT_SAME_TYPE(decltype(m.crend()), M::const_reverse_iterator);
    assert(m.size() == 8);
    assert(std::distance(m.begin(), m.end()) == 8);
    assert(std::distance(m.cbegin(), m.cend()) == 8);
    assert(std::distance(m.rbegin(), m.rend()) == 8);
    assert(std::distance(m.crbegin(), m.crend()) == 8);
    M::const_iterator i;  // default-construct 
    i = m.begin();  // move-assignment
    for (int j = 1; j <= 8; ++j, ++i) {
      assert(*i == j);  // pre-increment, dereference
    }
    assert(i == m.end());
    for (int j = 8; j >= 1; --j) {
      --i;  // pre-decrement
      assert(*i == j);     
    }
    assert(i == m.begin());
  }
  {
    // N3644 testing
    using C = std::flat_set<int>;
    C::iterator ii1{}, ii2{};
    C::iterator ii4 = ii1;
    C::const_iterator cii{};
    assert(ii1 == ii2);
    assert(ii1 == ii4);
    assert(!(ii1 != ii2));

    assert( (ii1 == cii));
    assert( (cii == ii1));
    assert(!(ii1 != cii));
    assert(!(cii != ii1));
  }
  return 0;
}
