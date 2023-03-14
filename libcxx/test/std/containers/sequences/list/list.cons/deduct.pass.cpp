//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <list>

// template <class InputIterator, class Allocator = allocator<typename iterator_traits<InputIterator>::value_type>>
//    list(InputIterator, InputIterator, Allocator = Allocator())
//    -> list<typename iterator_traits<InputIterator>::value_type, Allocator>;
//

#include <list>
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
    std::list lst(std::begin(arr), std::end(arr));
    ASSERT_SAME_TYPE(decltype(lst), std::list<int>);
    assert(std::equal(lst.begin(), lst.end(), std::begin(arr), std::end(arr)));
  }
  {
    const long arr[] = { INT_MAX, 1L, 2L, 3L };
    std::list lst(std::begin(arr), std::end(arr), std::allocator<long>());
    ASSERT_SAME_TYPE(decltype(lst), std::list<long>);
    assert((lst == std::list<long>{INT_MAX,1,2,3}));
  }
  {
    std::list lst(42, A{}); // (Count, T)
    ASSERT_SAME_TYPE(decltype(lst), std::list<A>);
    assert(lst.size() == 42);
  }
  {
    std::list lst(42, std::allocator<int>()); // (Count, T) again!
    ASSERT_SAME_TYPE(decltype(lst), std::list<std::allocator<int>>);
    assert(lst.size() == 42);
  }
  {
    std::list lst(42, A{}, test_allocator<A>(0, 43)); // (Count, T, Alloc)
    ASSERT_SAME_TYPE(decltype(lst), std::list<A, test_allocator<A>>);
    assert(lst.size() == 42);
    assert(lst.get_allocator().get_id() == 43);
  }
  {
    std::list lst = {1u, 2u, 3u, 4u, 5u}; // (initializer-list)
    ASSERT_SAME_TYPE(decltype(lst), std::list<unsigned>);
    assert((lst == std::list<unsigned>{1,2,3,4,5}));
  }
  {
    std::list lst({1.0, 2.0, 3.0, 4.0}, test_allocator<double>(0, 42)); // (initializer-list, Alloc)
    ASSERT_SAME_TYPE(decltype(lst), std::list<double, test_allocator<double>>);
    assert((lst == decltype(lst){1,2,3,4}));
    assert(lst.get_allocator().get_id() == 42);
  }
  {
    std::list<long double> source;
    std::list lst(source); // copy ctor
    ASSERT_SAME_TYPE(decltype(lst), std::list<long double>);
    assert(lst.size() == 0);
  }
  {
    typedef test_allocator<short> Alloc;
    typedef test_allocator<int> ConvertibleToAlloc;
    {
      std::list<short, Alloc> source;
      std::list lst(source, Alloc(2));
      static_assert(std::is_same_v<decltype(lst), decltype(source)>);
    }
    {
      std::list<short, Alloc> source;
      std::list lst(source, ConvertibleToAlloc(2));
      static_assert(std::is_same_v<decltype(lst), decltype(source)>);
    }
    {
      std::list<short, Alloc> source;
      std::list lst(std::move(source), Alloc(2));
      static_assert(std::is_same_v<decltype(lst), decltype(source)>);
    }
    {
      std::list<short, Alloc> source;
      std::list lst(std::move(source), ConvertibleToAlloc(2));
      static_assert(std::is_same_v<decltype(lst), decltype(source)>);
    }
  }

  SequenceContainerDeductionGuidesSfinaeAway<std::list, std::list<int>>();

  return true;
}

int main(int, char**) {
  tests();
  return 0;
}
