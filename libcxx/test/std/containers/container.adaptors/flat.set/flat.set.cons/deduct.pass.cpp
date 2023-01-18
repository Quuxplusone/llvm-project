//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

#include <algorithm>
#include <cassert>
#include <climits>
#include <deque>
#include <list>
#include <flat_set>
#include <functional>
#include <memory_resource>
#include <ranges>
#include <type_traits>
#include <vector>

#include "deduction_guides_sfinae_checks.h"
#include "test_allocator.h"

struct NotAnAllocator {
  friend bool operator<(NotAnAllocator, NotAnAllocator) { return false; }
};

void test_copy() {
  {
    std::flat_set<long> source = {1, 2, 3};
    std::flat_set s(source);
    ASSERT_SAME_TYPE(decltype(s), decltype(source));
    assert(s == source);
  }
  {
    std::flat_set<long, std::greater<long>> source = {1, 2, 3};
    std::flat_set s{ source };  // braces instead of parens
    ASSERT_SAME_TYPE(decltype(s), decltype(source));
    assert(s == source);
  }
#if 0 // LWG 3884, flat_foo is missing allocator-extended copy/move constructors
  {
    std::flat_set<long, std::greater<long>> source = {1, 2, 3};
    std::flat_set s(source, std::allocator<int>());
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<long, std::greater<long>>);
    assert(s == source);
  }
#endif
}

void test_container() {
  std::deque<int, test_allocator<int>> v({ 1, 2, 1, INT_MAX, 3 }, test_allocator<int>(0, 42));
  std::deque<int, test_allocator<int>> expected({ 1, 2, 3, INT_MAX }, test_allocator<int>(0, 42));
#if 0 // LWG ????, missing deduction guides for flat_foo's container constructors
  {
    std::flat_set s(v);

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::deque<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
  {
    std::flat_set s(std::sorted_unique, expected);

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::deque<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
  {
    std::flat_set s(v, test_allocator<long>(0, 43));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::deque<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().get_id() == 43);
  }
  {
    std::flat_set s(std::sorted_unique, expected, test_allocator<long>(0, 43));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::deque<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().get_id() == 43);
  }
#if 0 // LWG ????, Alloc parameter doesn't have to be an allocator
  {
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::monotonic_buffer_resource mr2;
    std::pmr::deque<int> pv({ 1, 2, 1, INT_MAX, 3 }, &mr);
    std::flat_set s(std::move(pv), &mr2);

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::pmr::deque<int>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().resource() == &mr2);
  }
  {
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::monotonic_buffer_resource mr2;
    std::pmr::deque<int> pv({ 1, 2, 3, INT_MAX }, &mr);
    std::flat_set s(std::sorted_unique, std::move(pv), &mr2);

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::pmr::deque<int>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().resource() == &mr2);
  }
#endif
#endif
}

void test_container_compare() {
  std::deque<int, test_allocator<int>> v({ 1, 2, 1, INT_MAX, 3 }, test_allocator<int>(0, 42));
  std::deque<int, test_allocator<int>> expected({ INT_MAX, 3, 2, 1 }, test_allocator<int>(0, 42));
#if 0 // LWG ????, missing deduction guides for flat_foo's container constructors
  {
    std::flat_set s(v, std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::deque<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
  {
    std::flat_set s(std::sorted_unique, expected, std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::deque<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
  {
    std::flat_set s(v, std::greater<int>(), test_allocator<long>(0, 43));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::deque<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().get_id() == 43);
  }
  {
    std::flat_set s(std::sorted_unique, expected, std::greater<int>(), test_allocator<long>(0, 43));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::deque<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().get_id() == 43);
  }
#if 0 // LWG ????, Alloc parameter doesn't have to be an allocator
  {
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::monotonic_buffer_resource mr2;
    std::pmr::deque<int> pv({ 1, 2, 1, INT_MAX, 3 }, &mr);
    std::flat_set s(std::move(pv), std::greater<int>(), &mr2);

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::pmr::deque<int>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().resource() == &mr2);
  }
  {
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::monotonic_buffer_resource mr2;
    std::pmr::deque<int> pv({ INT_MAX, 3, 2, 1 }, &mr);
    std::flat_set s(std::sorted_unique, std::move(pv), std::greater<int>(), &mr2);

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::pmr::deque<int>>);
    assert(std::ranges::equal(s, expected));
    assert(std::move(s).extract().get_allocator().resource() == &mr2);
  }
#endif
#endif
}

void test_iter_iter() {
  const int arr[] = { 1, 2, 1, INT_MAX, 3 };
  const int expected_s[] = { 1, 2, 3, INT_MAX };
  {
    std::flat_set s(arr, arr + 5);

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    assert(std::ranges::equal(s, expected_s));
  }
  {
    std::flat_set s(std::sorted_unique, expected_s, expected_s + 4);

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    assert(std::ranges::equal(s, expected_s));
  }
#if 0 // LWG 3804, flat_foo missing some allocator-extended deduction guides
  {
    std::flat_set s(arr, arr + 5, test_allocator<int>(0, 42));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
  {
    std::flat_set s(std::sorted_unique, expected_s, expected_s + 4, test_allocator<int>(0, 42));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
#endif
}

void test_iter_iter_compare() {
  const int arr[] = { 1, 2, 1, INT_MAX, 3 };
  const int expected_s[] = { INT_MAX, 3, 2, 1 };
  {
    std::flat_set s(arr, arr + 5, std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>>);
    assert(std::ranges::equal(s, expected_s));
  }
  {
    std::flat_set s(std::sorted_unique, expected_s, expected_s + 4, std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>>);
    assert(std::ranges::equal(s, expected_s));
  }
#if 0 // LWG 3804, flat_foo missing some allocator-extended deduction guides
  {
    std::flat_set s(arr, arr + 5, std::greater<int>(), test_allocator<int>(0, 42));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
  {
    std::flat_set s(std::sorted_unique, expected_s, expected_s + 4, std::greater<int>(), test_allocator<int>(0, 42));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
#endif
}

void test_initializer_list() {
  const int expected_s[] = { 1, 2, 3, INT_MAX };
  {
    std::flat_set s{ 1, 2, 1, INT_MAX, 3 };

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    assert(std::ranges::equal(s, expected_s));
  }
  {
    std::flat_set s(std::sorted_unique, { 1, 2, 3, INT_MAX });

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int>);
    assert(std::ranges::equal(s, expected_s));
  }
#if 0 // LWG 3804, flat_foo missing some allocator-extended deduction guides
  {
    std::flat_set s({ 1, 2, 1, INT_MAX, 3 }, test_allocator<long>(0, 42));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
  {
    std::flat_set s(std::sorted_unique, { 1, 2, 3, INT_MAX }, test_allocator<long>(0, 42));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
#endif
}

void test_initializer_list_compare() {
  const int expected_s[] = { INT_MAX, 3, 2, 1 };
  {
    std::flat_set s({ 1, 2, 1, INT_MAX, 3 }, std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>>);
    assert(std::ranges::equal(s, expected_s));
  }
  {
    std::flat_set s(std::sorted_unique, { INT_MAX, 3, 2, 1 }, std::greater<int>());

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>>);
    assert(std::ranges::equal(s, expected_s));
  }
#if 0 // LWG 3804, flat_foo missing some allocator-extended deduction guides
  {
    std::flat_set s({ 1, 2, 1, INT_MAX, 3 }, std::greater<int>(), test_allocator<long>(0, 42));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
  {
    std::flat_set s(std::sorted_unique, { INT_MAX, 3, 2, 1 }, std::greater<int>(), test_allocator<long>(0, 42));

    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
#endif
}

void test_from_range() {
  std::list<int> r = { 1, 2, 1, INT_MAX, 3 };
  const int expected_s[] = { 1, 2, 3, INT_MAX };
  {
    std::flat_set s(std::from_range, r);
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>>);
    assert(std::ranges::equal(s, expected_s));
  }
#if 0 // LWG 3786, flat maps' deduction guide needs to default Allocator to be useful
  {
    std::flat_set s(std::from_range, r, test_allocator<long>(0, 42));
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::less<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
#endif
}

void test_from_range_compare() {
  std::list<int> r = { 1, 2, 1, INT_MAX, 3 };
  const int expected_s[] = { INT_MAX, 3, 2, 1 };
  {
    std::flat_set s(std::from_range, r, std::greater<int>());
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>>);
    assert(std::ranges::equal(s, expected_s));
  }
#if 0 // LWG 3786, flat maps' deduction guide needs to default Allocator to be useful
  {
    std::flat_set s(std::from_range, r, std::greater<int>(), test_allocator<long>(0, 42));
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<int, std::greater<int>, std::vector<int, test_allocator<int>>>);
    assert(std::ranges::equal(s, expected_s));
    assert(std::move(s).extract().get_allocator().get_id() == 42);
  }
#endif
}

int main(int, char **)
{
  // Each test function also tests the sorted_unique-prefixed and allocator-suffixed overloads.
  test_copy();
  test_container();
  test_container_compare();
  test_iter_iter();
  test_iter_iter_compare();
  test_initializer_list();
  test_initializer_list_compare();
  test_from_range();
  test_from_range_compare();

#if 0 // LWG 3804, flat_foo missing some allocator-extended deduction guides
  AssociativeContainerDeductionGuidesSfinaeAway<std::flat_set, std::flat_set<int>>();
#endif

  {
    NotAnAllocator a;
    std::flat_set s{ a }; // flat_set(initializer_list<NotAnAllocator>)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<NotAnAllocator>);
    assert(s.size() == 1);
  }

  {
    std::flat_set<long> source;
    std::flat_set s{ source, source }; // flat_set(initializer_list<flat_set<long>>)
    ASSERT_SAME_TYPE(decltype(s), std::flat_set<std::flat_set<long> >);
    assert(s.size() == 1);
  }

  {
    NotAnAllocator a;
    std::flat_set s{ a, a }; // flat_set(initializer_list<NotAnAllocator>)
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

  return 0;
}
