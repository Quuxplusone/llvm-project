//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <forward_list>

// template <class InputIterator, class Allocator = allocator<typename iterator_traits<InputIterator>::value_type>>
//    forward_list(InputIterator, InputIterator, Allocator = Allocator())
//    -> forward_list<typename iterator_traits<InputIterator>::value_type, Allocator>;
//

#include <forward_list>
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

bool tests() {
  {
    const int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::forward_list fwl(std::begin(arr), std::end(arr));
    ASSERT_SAME_TYPE(decltype(fwl), std::forward_list<int>);
    assert(std::equal(fwl.begin(), fwl.end(), std::begin(arr), std::end(arr)));
  }
  {
    const long arr[] = { INT_MAX, 1L, 2L, 3L };
    std::forward_list fwl(std::begin(arr), std::end(arr), std::allocator<long>());
    ASSERT_SAME_TYPE(decltype(fwl), std::forward_list<long>);
    assert((fwl == std::forward_list<long>{INT_MAX,1,2,3}));
  }
  {
    std::forward_list fwl(42, A{}); // (Count, T)
    ASSERT_SAME_TYPE(decltype(fwl), std::forward_list<A>);
    assert(std::distance(fwl.begin(), fwl.end()) == 42);
  }
  {
    std::forward_list fwl(42, std::allocator<int>()); // (Count, T) again!
    ASSERT_SAME_TYPE(decltype(fwl), std::forward_list<std::allocator<int>>);
    assert(std::distance(fwl.begin(), fwl.end()) == 42);
  }
  {
    std::forward_list fwl(42, A{}, test_allocator<A>(0, 43)); // (Count, T, Alloc)
    ASSERT_SAME_TYPE(decltype(fwl), std::forward_list<A, test_allocator<A>>);
    assert(std::distance(fwl.begin(), fwl.end()) == 42);
    assert(fwl.get_allocator().get_id() == 43);
  }
  {
    std::forward_list fwl = {1u, 2u, 3u, 4u, 5u}; // (initializer-forward_list)
    ASSERT_SAME_TYPE(decltype(fwl), std::forward_list<unsigned>);
    assert((fwl == std::forward_list<unsigned>{1,2,3,4,5}));
  }
  {
    std::forward_list fwl({1.0, 2.0, 3.0, 4.0}, test_allocator<double>(0, 42)); // (initializer-forward_list, Alloc)
    ASSERT_SAME_TYPE(decltype(fwl), std::forward_list<double, test_allocator<double>>);
    assert((fwl == decltype(fwl){1,2,3,4}));
    assert(fwl.get_allocator().get_id() == 42);
  }
  {
    std::forward_list<long double> source;
    std::forward_list fwl(source); // copy ctor
    ASSERT_SAME_TYPE(decltype(fwl), std::forward_list<long double>);
    assert(fwl.empty());
  }
  {
    typedef test_allocator<short> Alloc;
    typedef test_allocator<int> ConvertibleToAlloc;
    {
      std::forward_list<short, Alloc> source;
      std::forward_list fwl(source, Alloc(2));
      static_assert(std::is_same_v<decltype(fwl), decltype(source)>);
    }
    {
      std::forward_list<short, Alloc> source;
      std::forward_list fwl(source, ConvertibleToAlloc(2));
      static_assert(std::is_same_v<decltype(fwl), decltype(source)>);
    }
    {
      std::forward_list<short, Alloc> source;
      std::forward_list fwl(std::move(source), Alloc(2));
      static_assert(std::is_same_v<decltype(fwl), decltype(source)>);
    }
    {
      std::forward_list<short, Alloc> source;
      std::forward_list fwl(std::move(source), ConvertibleToAlloc(2));
      static_assert(std::is_same_v<decltype(fwl), decltype(source)>);
    }
  }

  SequenceContainerDeductionGuidesSfinaeAway<std::forward_list, std::forward_list<int>>();

  return true;
}

int main(int, char**) {
  tests();
  return 0;
}
