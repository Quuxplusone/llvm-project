//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <vector>

// template <class InputIterator, class Allocator = allocator<typename iterator_traits<InputIterator>::value_type>>
//    vector(InputIterator, InputIterator, Allocator = Allocator())
//    -> vector<typename iterator_traits<InputIterator>::value_type, Allocator>;
//

#include <vector>
#include <algorithm>
#include <cassert>
#include <climits> // INT_MAX
#include <iterator>
#include <memory>

#include "deduction_guides_sfinae_checks.h"
#include "test_macros.h"
#include "test_iterators.h"
#include "test_allocator.h"

struct A {};

TEST_CONSTEXPR_CXX20 bool tests() {
  {
    const int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector vec(std::begin(arr), std::end(arr));
    ASSERT_SAME_TYPE(decltype(vec), std::vector<int>);
    assert(std::equal(vec.begin(), vec.end(), std::begin(arr), std::end(arr)));
  }
  {
    const long arr[] = { INT_MAX, 1L, 2L, 3L };
    std::vector vec(std::begin(arr), std::end(arr), std::allocator<long>());
    ASSERT_SAME_TYPE(decltype(vec), std::vector<long>);
    assert((vec == std::vector<long>{INT_MAX,1,2,3}));
  }
  {
    std::vector vec(42, A{}); // (Count, T)
    ASSERT_SAME_TYPE(decltype(vec), std::vector<A>);
    assert(vec.size() == 42);
  }
  {
    std::vector vec(42, std::allocator<int>()); // (Count, T) again!
    ASSERT_SAME_TYPE(decltype(vec), std::vector<std::allocator<int>>);
    assert(vec.size() == 42);
  }
  {
    std::vector vec(42, A{}, test_allocator<A>(0, 43)); // (Count, T, Alloc)
    ASSERT_SAME_TYPE(decltype(vec), std::vector<A, test_allocator<A>>);
    assert(vec.size() == 42);
    assert(vec.get_allocator().get_id() == 43);
  }
  {
    std::vector vec = {1u, 2u, 3u, 4u, 5u}; // (initializer-list)
    ASSERT_SAME_TYPE(decltype(vec), std::vector<unsigned>);
    assert((vec == std::vector<unsigned>{1,2,3,4,5}));
  }
  {
    std::vector vec({1.0, 2.0, 3.0, 4.0}, test_allocator<double>(0, 42)); // (initializer-list, Alloc)
    ASSERT_SAME_TYPE(decltype(vec), std::vector<double, test_allocator<double>>);
    assert((vec == decltype(vec){1,2,3,4}));
    assert(vec.get_allocator().get_id() == 42);
  }
  {
    std::vector<long double> source;
    std::vector vec(source); // copy ctor
    ASSERT_SAME_TYPE(decltype(vec), std::vector<long double>);
    assert(vec.size() == 0);
  }

  // A couple of vector<bool> tests, too!
  {
    std::vector vec(3, true); // (Count, T)
    ASSERT_SAME_TYPE(decltype(vec), std::vector<bool>);
    assert(vec.size() == 3);
    assert(vec[0] && vec[1] && vec[2]);
  }
  {
    std::vector<bool> source;
    std::vector vec(source); // copy ctor
    ASSERT_SAME_TYPE(decltype(vec), std::vector<bool>);
    assert(vec.size() == 0);
  }
  {
    typedef test_allocator<short> Alloc;
    typedef test_allocator<int> ConvertibleToAlloc;
    {
      std::vector<short, Alloc> source;
      std::vector vec(source, Alloc(2));
      static_assert(std::is_same_v<decltype(vec), decltype(source)>);
    }
    {
      std::vector<short, Alloc> source;
      std::vector vec(source, ConvertibleToAlloc(2));
      static_assert(std::is_same_v<decltype(vec), decltype(source)>);
    }
    {
      std::vector<short, Alloc> source;
      std::vector vec(std::move(source), Alloc(2));
      static_assert(std::is_same_v<decltype(vec), decltype(source)>);
    }
    {
      std::vector<short, Alloc> source;
      std::vector vec(std::move(source), ConvertibleToAlloc(2));
      static_assert(std::is_same_v<decltype(vec), decltype(source)>);
    }
  }

  SequenceContainerDeductionGuidesSfinaeAway<std::vector, std::vector<int>>();

  return true;
}

int main(int, char**) {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return 0;
}
