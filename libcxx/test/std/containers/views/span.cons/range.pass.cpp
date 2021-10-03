//===---------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-has-no-incomplete-ranges

// <span>

//  template<class R>
//    constexpr explicit(extent != dynamic_extent) span(R&& r);
//
// Let U be remove_reference_t<ranges::range_reference_t<R>>.
// Remarks: This constructor shall not participate in overload resolution unless:
//   - R satisfies ranges::contiguous_range and ranges::sized_range,
//   - Either R satisfies ranges::borrowed_range or is_const_v<element_type> is true,
//   - remove_cvref_t<R> is not a specialization of span,
//   - remove_cvref_t<R> is not a specialization of array,
//   — is_array_v<remove_cvref_t<R>> is false,
//   - is_convertible_v<rU(*)[], element_type(*)[] is true


#include <span>
#include <cassert>
#include <deque>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <vector>

template <class T, size_t Extent>
constexpr bool test_from_range() {
  T val[3]{};
  std::span<T, Extent> s{val};
  assert(s.size() == std::size(val));
  assert(s.data() == std::data(val));
  return true;
}

struct A {};

constexpr bool test() {
  assert((test_from_range<int, std::dynamic_extent>()));
  assert((test_from_range<int, 3>()));
  assert((test_from_range<A, std::dynamic_extent>()));
  assert((test_from_range<A, 3>()));
  return true;
}

static_assert(!std::is_constructible_v<std::span<int>, std::vector<float>&>);    // wrong type
static_assert(!std::is_constructible_v<std::span<int, 3>, std::vector<float>&>); // wrong type

static_assert(std::is_constructible_v<std::span<int>, std::vector<int>&>);                // non-borrowed lvalue
static_assert(std::is_constructible_v<std::span<int, 3>, std::vector<int>&>);             // non-borrowed lvalue
static_assert(!std::is_constructible_v<std::span<int>, const std::vector<int>&>);         // non-borrowed const lvalue
static_assert(!std::is_constructible_v<std::span<int, 3>, const std::vector<int>&>);      // non-borrowed const lvalue
static_assert(std::is_constructible_v<std::span<const int>, const std::vector<int>&>);    // non-borrowed const lvalue
static_assert(std::is_constructible_v<std::span<const int, 3>, const std::vector<int>&>); // non-borrowed const lvalue
static_assert(std::is_constructible_v<std::span<const int>, std::vector<int>&>);          // non-borrowed lvalue
static_assert(std::is_constructible_v<std::span<const int, 3>, std::vector<int>&>);       // non-borrowed lvalue
static_assert(!std::is_constructible_v<std::span<int>, std::vector<int>&&>);              // non-borrowed rvalue
static_assert(!std::is_constructible_v<std::span<int, 3>, std::vector<int>&&>);           // non-borrowed rvalue
static_assert(!std::is_constructible_v<std::span<int>, std::deque<int>&>);                // non-contiguous lvalue
static_assert(!std::is_constructible_v<std::span<int, 3>, std::deque<int>&>);             // non-contiguous lvalue
static_assert(!std::is_constructible_v<
              std::span<int>, std::ranges::subrange<std::deque<int>::iterator,
                                                    std::deque<int>::iterator>&&>); // non-contiguous borrowed rvalue
static_assert(!std::is_constructible_v<
              std::span<int, 3>, std::ranges::subrange<std::deque<int>::iterator,
                                                       std::deque<int>::iterator>&&>); // non-contiguous borrowed rvalue

using BorrowedContiguousSizedRange = std::string_view;
static_assert(std::is_constructible_v<std::span<const char>, BorrowedContiguousSizedRange>);
static_assert(std::is_constructible_v<std::span<const char, 3>, BorrowedContiguousSizedRange>);
static_assert(!std::is_constructible_v<std::span<char>, BorrowedContiguousSizedRange>);
static_assert(!std::is_constructible_v<std::span<char, 3>, BorrowedContiguousSizedRange>);

static_assert(std::is_convertible_v<BorrowedContiguousSizedRange&, std::span<const char> >);
static_assert(!std::is_convertible_v<BorrowedContiguousSizedRange&, std::span<const char, 3> >);
static_assert(!std::is_convertible_v<BorrowedContiguousSizedRange&, std::span<char> >);
static_assert(!std::is_convertible_v<BorrowedContiguousSizedRange&, std::span<char, 3> >);
static_assert(std::is_convertible_v<const BorrowedContiguousSizedRange&, std::span<const char> >);
static_assert(!std::is_convertible_v<const BorrowedContiguousSizedRange&, std::span<const char, 3> >);

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
