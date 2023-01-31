//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

#include <hive>
#include <algorithm>
#include <cassert>
#include <climits> // INT_MAX
#include <iterator>
#include <memory>
#include <memory_resource>

#include "deduction_guides_sfinae_checks.h"
#include "test_macros.h"
#include "test_iterators.h"
#include "test_allocator.h"

struct A {};

bool tests() {
  {
    const int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::hive h(std::begin(arr), std::end(arr));
    ASSERT_SAME_TYPE(decltype(h), std::hive<int>);
    assert(std::is_permutation(h.begin(), h.end(), arr, arr + 10));
  }
  {
    const long arr[] = { INT_MAX, 1L, 2L, 3L };
    std::hive h(std::begin(arr), std::end(arr), test_allocator<long>(0, 42));
    ASSERT_SAME_TYPE(decltype(h), std::hive<long, test_allocator<long>>);
    assert(std::is_permutation(h.begin(), h.end(), arr, arr + 4));
    assert(h.get_allocator().get_id() == 42);
  }
  {
    std::hive h(42, A{}); // (Count, T)
    ASSERT_SAME_TYPE(decltype(h), std::hive<A>);
    assert(h.size() == 42);
  }
  {
    std::hive h(42, std::allocator<int>()); // (Count, T) again!
    ASSERT_SAME_TYPE(decltype(h), std::hive<std::allocator<int>>);
    assert(h.size() == 42);
  }
  {
    std::hive h(42, A{}, test_allocator<A>(0, 43)); // (Count, T, Alloc)
    ASSERT_SAME_TYPE(decltype(h), std::hive<A, test_allocator<A>>);
    assert(h.size() == 42);
    assert(h.get_allocator().get_id() == 43);
  }
  {
    unsigned expected[] = {1u, 2u, 3u, 4u, 5u};
    std::hive h = {1u, 2u, 3u, 4u, 5u}; // (initializer-hive)
    ASSERT_SAME_TYPE(decltype(h), std::hive<unsigned>);
    assert(std::is_permutation(h.begin(), h.end(), expected, expected + 5));
  }
  {
    double expected[] = {1, 2, 3, 4};
    std::hive h({1.0, 2.0, 3.0, 4.0}, test_allocator<double>(0, 42)); // (initializer-hive, Alloc)
    ASSERT_SAME_TYPE(decltype(h), std::hive<double, test_allocator<double>>);
    assert(std::is_permutation(h.begin(), h.end(), expected, expected + 4));
    assert(h.get_allocator().get_id() == 42);
  }
#if TEST_STD_VER >= 23
  {
    short a[] = {1,2,3,4,5};
    std::hive h(std::from_range, a); // (from_range_t, InputRange)
    ASSERT_SAME_TYPE(decltype(h), std::hive<short>);
    assert(std::ranges::is_permutation(h, a));
  }
  {
    short a[] = {1,2,3,4,5};
    std::hive h(std::from_range, a, test_allocator<short>(0, 42)); // (from_range_t, InputRange, Alloc)
    ASSERT_SAME_TYPE(decltype(h), std::hive<short, test_allocator<short>>);
    assert(std::ranges::is_permutation(h, a));
    assert(h.get_allocator().get_id() == 42);
  }
#endif
  {
    std::hive<long double> source;
    std::hive h(source); // copy ctor
    ASSERT_SAME_TYPE(decltype(h), std::hive<long double>);
    assert(h.size() == 0);
  }
  {
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::hive<short> source;
    std::hive h(source, &mr); // allocator-extended copy ctor
    ASSERT_SAME_TYPE(decltype(h), std::pmr::hive<short>);
    assert(h.get_allocator().resource() == &mr);
  }
  {
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::hive<short> source;
    std::hive h(std::move(source), &mr); // allocator-extended move ctor
    ASSERT_SAME_TYPE(decltype(h), std::pmr::hive<short>);
    assert(h.get_allocator().resource() == &mr);
  }

  SequenceContainerDeductionGuidesSfinaeAway<std::hive, std::hive<int>>();

  return true;
}

int main(int, char**) {
  tests();
  return 0;
}
