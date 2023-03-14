//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <deque>

// template <class InputIterator, class Allocator = allocator<typename iterator_traits<InputIterator>::value_type>>
//    deque(InputIterator, InputIterator, Allocator = Allocator())
//    -> deque<typename iterator_traits<InputIterator>::value_type, Allocator>;
//
// template<ranges::input_range R, class Allocator = allocator<ranges::range_value_t<R>>>
//   deque(from_range_t, R&&, Allocator = Allocator())
//     -> deque<ranges::range_value_t<R>, Allocator>; // C++23

#include <cassert>
#include <climits> // INT_MAX
#include <deque>

#include "asan_testing.h"
#include "deduction_guides_sfinae_checks.h"
#include "test_macros.h"
#include "test_iterators.h"
#include "test_allocator.h"

struct A {};

bool tests() {
  {
    const int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::deque deq(arr, arr + 10);
    ASSERT_SAME_TYPE(decltype(deq), std::deque<int>);
    assert(std::equal(deq.begin(), deq.end(), std::begin(arr), std::end(arr)));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
  }
  {
    const long arr[] = { INT_MAX, 1L, 2L, 3L };
    std::deque deq(arr, arr + 4, std::allocator<long>());
    ASSERT_SAME_TYPE(decltype(deq), std::deque<long>);
    assert((deq == std::deque<long>{INT_MAX,1,2,3}));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
  }
  {
    std::deque deq(42, A{}); // (Count, T)
    ASSERT_SAME_TYPE(decltype(deq), std::deque<A>);
    assert(deq.size() == 42);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
  }
  {
    std::deque deq(42, std::allocator<int>()); // (Count, T) again!
    ASSERT_SAME_TYPE(decltype(deq), std::deque<std::allocator<int>>);
    assert(deq.size() == 42);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
  }
  {
    std::deque deq(42, A{}, test_allocator<A>(0, 43)); // (Count, T, Alloc)
    ASSERT_SAME_TYPE(decltype(deq), std::deque<A, test_allocator<A>>);
    assert(deq.size() == 42);
    assert(deq.get_allocator().get_id() == 43);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
  }
  {
    std::deque deq = {1u, 2u, 3u, 4u, 5u}; // (initializer-list)
    ASSERT_SAME_TYPE(decltype(deq), std::deque<unsigned>);
    assert((deq == std::deque<unsigned>{1,2,3,4,5}));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
  }
  {
    std::deque deq({1.0, 2.0, 3.0, 4.0}, test_allocator<double>(0, 42)); // (initializer-list, Alloc)
    ASSERT_SAME_TYPE(decltype(deq), std::deque<double, test_allocator<double>>);
    assert((deq == decltype(deq){1,2,3,4}));
    assert(deq.get_allocator().get_id() == 42);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
  }
  {
    std::deque<long double> source;
    std::deque deq(source); // copy ctor
    ASSERT_SAME_TYPE(decltype(deq), std::deque<long double>);
    assert(deq.size() == 0);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(source));
  }
  {
    typedef test_allocator<short> Alloc;
    typedef test_allocator<int> ConvertibleToAlloc;
    {
      std::deque<short, Alloc> source;
      std::deque deq(source, Alloc(2));
      static_assert(std::is_same_v<decltype(deq), decltype(source)>);
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(source));
    }
    {
      std::deque<short, Alloc> source;
      std::deque deq(source, ConvertibleToAlloc(2));
      static_assert(std::is_same_v<decltype(deq), decltype(source)>);
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(source));
    }
    {
      std::deque<short, Alloc> source;
      std::deque deq(std::move(source), Alloc(2));
      static_assert(std::is_same_v<decltype(deq), decltype(source)>);
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(source));
    }
    {
      std::deque<short, Alloc> source;
      std::deque deq(std::move(source), ConvertibleToAlloc(2));
      static_assert(std::is_same_v<decltype(deq), decltype(source)>);
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(source));
    }
  }
#if TEST_STD_VER >= 23
  {
    {
      int a[2] = {1,2};
      std::deque deq(std::from_range, a);
      ASSERT_SAME_TYPE(decltype(deq), std::deque<int>);
      assert(deq.size() == 2);
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
    }
    {
      using Alloc = test_allocator<int>;
      int a[2] = {1,2};
      std::deque deq(std::from_range, a, Alloc());
      ASSERT_SAME_TYPE(decltype(deq), std::deque<int, Alloc>);
      assert(deq.size() == 2);
      LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(deq));
    }
  }
#endif

  SequenceContainerDeductionGuidesSfinaeAway<std::deque, std::deque<int>>();

  return true;
}

int main(int, char**) {
  tests();
  return 0;
}
