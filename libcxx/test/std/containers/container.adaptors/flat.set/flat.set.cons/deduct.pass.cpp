//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// template<class InputIterator, class Compare = less<iter-value-type<InputIterator>>>
// flat_set(InputIterator, InputIterator, Compare = Compare())
//   -> flat_set<iter-value-type<InputIterator>, Compare>;
//
// template<class InputIterator, class Compare = less<iter-value-type<InputIterator>>>
//   flat_set(sorted_unique_t, InputIterator, InputIterator, Compare = Compare())
//     -> flat_set<iter-value-type<InputIterator>, Compare>;
//
// template<ranges::input_range R, class Compare = less<ranges::range_value_t<R>>,
//         class Allocator = allocator<ranges::range_value_t<R>>>
//   flat_set(from_range_t, R&&, Compare = Compare(), Allocator = Allocator())
//     -> flat_set<ranges::range_value_t<R>, Compare>;
//
//  template<ranges::input_range R, class Allocator>
//    flat_set(from_range_t, R&&, Allocator)
//      -> flat_set<ranges::range_value_t<R>, less<ranges::range_value_t<R>>>;
//
// template<class Key, class Compare = less<Key>>
//   flat_set(initializer_list<Key>, Compare = Compare())
//     -> flat_set<Key, Compare>;
//
// template<class Key, class Compare = less<Key>>
//   flat_set(sorted_unique_t, initializer_list<Key>, Compare = Compare())
//     -> flat_set<Key, Compare>;

#include <algorithm> // std::equal
#include <cassert>
#include <climits> // INT_MAX
#include <list>
#include <flat_set>
#include <functional>
#include <type_traits>

#include "deduction_guides_sfinae_checks.h"
#include "test_allocator.h"

struct NotAnAllocator {
  friend bool operator<(NotAnAllocator, NotAnAllocator) { return false; }
};

int main(int, char **) {
  {
    const int arr[] = { 1, 2, 1, INT_MAX, 3 };
    std::flat_set s(std::begin(arr), std::end(arr));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }
  {
    const int arr[] = { 1, 2, 3, INT_MAX };
    std::flat_set s(std::sorted_unique, std::begin(arr), std::end(arr));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }

  {
    const int arr[] = { 1, 2, 1, INT_MAX, 3 };
    std::flat_set s(std::begin(arr), std::end(arr), std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int> >);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }
  {
    const int arr[] = { INT_MAX, 3, 2, 1 };
    std::flat_set s(std::sorted_unique, std::begin(arr), std::end(arr), std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int> >);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }

#if 0 // LWG 3804
  {
    const int arr[] = { 1, 2, 1, INT_MAX, 3 };
    std::flat_set s(std::begin(arr), std::end(arr), std::greater<int>(),
                    test_allocator<int>(0, 42));

    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::greater<int>, test_allocator<int> >);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(s.get_allocator().get_id() == 42);
  }
  {
    const int arr[] = { INT_MAX, 3, 2, 1 };
    std::flat_set s(std::sorted_unique, std::begin(arr), std::end(arr), std::greater<int>(),
                    test_allocator<int>(0, 42));

    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::greater<int>, test_allocator<int> >);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(s.get_allocator().get_id() == 42);
  }
#endif

  {
    std::flat_set<long> source;
    std::flat_set s(source);
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<long>);
    assert(s.size() == 0);
  }

  {
    std::flat_set<long> source;
    std::flat_set s{ source };  // braces instead of parens
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<long>);
    assert(s.size() == 0);
  }

  {
    std::flat_set<long> source;
    std::flat_set s(source, std::allocator<long>());
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<long>);
    assert(s.size() == 0);
  }

  {
    std::flat_set s{ 1, 2, 1, INT_MAX, 3 };

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }
  {
    std::flat_set s(std::sorted_unique, { 1, 2, 3, INT_MAX });

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }

  {
    std::flat_set s({ 1, 2, 1, INT_MAX, 3 }, std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int> >);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }
  {
    std::flat_set s(std::sorted_unique, { INT_MAX, 3, 2, 1 }, std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int> >);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }

  {
    std::list<int> v = { 1, 2, 1, INT_MAX, 3 };
    std::flat_set s(std::from_range, v);
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>>);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }

  {
    std::list<int> v = { 1, 2, 1, INT_MAX, 3 };
    std::flat_set s(std::from_range, v, std::greater<int>());
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>>);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
  }

#if 0 // LWG 3804
  {
    std::flat_set s({ 1, 2, 1, INT_MAX, 3 }, std::greater<int>(),
                    test_allocator<int>(0, 43));

    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::greater<int>, test_allocator<int> >);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(s.get_allocator().get_id() == 43);
  }
  {
    std::flat_set s(std::sorted_unique, { INT_MAX, 3, 2, 1 }, std::greater<int>(),
                    test_allocator<int>(0, 43));

    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::greater<int>, test_allocator<int> >);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(s.get_allocator().get_id() == 43);
  }

  {
    const int arr[] = { 1, 2, 1, INT_MAX, 3 };
    std::flat_set s(std::begin(arr), std::end(arr), test_allocator<int>(0, 44));

    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::less<int>, test_allocator<int> >);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(s.get_allocator().get_id() == 44);
  }
  {
    const int arr[] = { 1, 2, 3, INT_MAX };
    std::flat_set s(std::sorted_unique, std::begin(arr), std::end(arr), test_allocator<int>(0, 44));

    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::less<int>, test_allocator<int> >);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(s.get_allocator().get_id() == 44);
  }

  {
    std::flat_set s({ 1, 2, 1, INT_MAX, 3 }, test_allocator<int>(0, 45));

    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::less<int>, test_allocator<int> >);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(s.get_allocator().get_id() == 45);
  }
  {
    std::flat_set s(std::sorted_unique, { 1, 2, 3, INT_MAX }, test_allocator<int>(0, 45));

    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::less<int>, test_allocator<int> >);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(s.get_allocator().get_id() == 45);
  }
#endif

  {
    std::list<int> v = { 1, 2, 1, INT_MAX, 3 };
    std::flat_set s(std::from_range, v, test_allocator<int>(0, 45));
    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::less<int>, std::vector<int, test_allocator<int>>>);
    const int expected_s[] = { 1, 2, 3, INT_MAX };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(std::move(s).extract().get_allocator().get_id() == 45);
  }

  {
    std::list<int> v = { 1, 2, 1, INT_MAX, 3 };
    std::flat_set s(std::from_range, v, std::greater<int>(), test_allocator<int>(0, 45));
    ASSERT_SAME_TYPE(decltype(s),
                     std::flat_set<int, std::greater<int>, std::vector<int, test_allocator<int>>>);
    const int expected_s[] = { INT_MAX, 3, 2, 1 };
    assert(std::equal(s.begin(), s.end(), std::begin(expected_s),
                      std::end(expected_s)));
    assert(std::move(s).extract().get_allocator().get_id() == 45);
  }

  {
    NotAnAllocator a;
    std::flat_set s{ a }; // set(initializer_list<NotAnAllocator>)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<NotAnAllocator>);
    assert(s.size() == 1);
  }

  {
    std::flat_set<long> source;
    std::flat_set s{ source, source }; // set(initializer_list<flat_set<long>>)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<std::flat_set<long> >);
    assert(s.size() == 1);
  }

  {
    NotAnAllocator a;
    std::flat_set s{ a, a }; // set(initializer_list<NotAnAllocator>)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<NotAnAllocator>);
    assert(s.size() == 1);
  }

  {
    int source[3] = { 3, 4, 5 };
    std::flat_set s(source, source + 3); // flat_set(InputIterator, InputIterator)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    assert(s.size() == 3);
  }
  {
    int source[3] = { 3, 4, 5 };
    std::flat_set s(std::sorted_unique, source, source + 3); // flat_set(sorted_unique_t, InputIterator, InputIterator)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    assert(s.size() == 3);
  }

  {
    int source[3] = { 3, 4, 5 };
    std::flat_set s{ source, source + 3 }; // flat_set(initializer_list<int*>)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int *>);
    assert(s.size() == 2);
  }

  {
    int source[3] = { 3, 4, 5 };
    std::flat_set s{ std::sorted_unique, source, source + 3 }; // flat_set(sorted_unique_t, InputIterator, InputIterator)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    assert(s.size() == 3);
  }

#if 0 // LWG 3804
  AssociativeContainerDeductionGuidesSfinaeAway<std::flat_set, std::flat_set<int>>();
#endif

  return 0;
}
