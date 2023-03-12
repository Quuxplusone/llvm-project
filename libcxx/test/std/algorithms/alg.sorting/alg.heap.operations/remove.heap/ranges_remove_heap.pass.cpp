//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20, c++23

// <algorithm>

// template<random_access_iterator I, sentinel_for<I> S, class Comp = ranges::less,
//         class Proj = identity>
//   requires sortable<I, Comp, Proj>
//   constexpr I
//     ranges::remove_heap(I first, S last, Comp comp = {}, Proj proj = {});                    // since C++26
//
// template<random_access_range R, class Comp = ranges::less, class Proj = identity>
//   requires sortable<iterator_t<R>, Comp, Proj>
//   constexpr typename borrowed_iterator_t<R>
//     ranges::remove_heap(R&& r, Comp comp = {}, Proj proj = {});                              // since C++26

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <functional>
#include <memory>
#include <ranges>

#include "almost_satisfies_types.h"
#include "test_iterators.h"

// SFINAE tests.

using BadComparator = ComparatorNotCopyable<int*>;
static_assert(!std::sortable<int*, BadComparator>);

template <class Iter, class Sent = sentinel_wrapper<Iter>, class Comp = std::ranges::less>
concept HasRemoveHeapIt = requires(Iter first, Sent last, Comp comp) { std::ranges::make_heap(first, last, comp); };

static_assert(HasRemoveHeapIt<int*>);
static_assert(!HasRemoveHeapIt<RandomAccessIteratorNotDerivedFrom>);
static_assert(!HasRemoveHeapIt<RandomAccessIteratorBadIndex>);
static_assert(!HasRemoveHeapIt<int*, SentinelForNotSemiregular>);
static_assert(!HasRemoveHeapIt<int*, SentinelForNotWeaklyEqualityComparableWith>);
static_assert(!HasRemoveHeapIt<int*, int*, BadComparator>);
static_assert(!HasRemoveHeapIt<const int*>); // Doesn't satisfy `sortable`.

template <class Range, class Comp = std::ranges::less>
concept HasRemoveHeapR = requires(Range range, Comp comp) { std::ranges::make_heap(range, comp); };

static_assert(HasRemoveHeapR<UncheckedRange<int*>>);
static_assert(!HasRemoveHeapR<RandomAccessRangeNotDerivedFrom>);
static_assert(!HasRemoveHeapR<RandomAccessRangeBadIndex>);
static_assert(!HasRemoveHeapR<UncheckedRange<int*, SentinelForNotSemiregular>>);
static_assert(!HasRemoveHeapR<UncheckedRange<int*, SentinelForNotWeaklyEqualityComparableWith>>);
static_assert(!HasRemoveHeapR<UncheckedRange<int*>, BadComparator>);
static_assert(!HasRemoveHeapR<UncheckedRange<const int*>>); // Doesn't satisfy `sortable`.

template <std::size_t N, class T, class Iter>
constexpr void verify_heap(const std::array<T, N>& heapified, Iter last, std::array<T, N> expected) {
  assert(std::equal(heapified.begin(), heapified.end() - 1, expected.begin()));
  assert(std::to_address(base(last)) == heapified.data() + heapified.size());
  assert(std::is_heap(heapified.begin(), heapified.end() - 1));
}

template <class Iter, class Sent, std::size_t N>
constexpr void test_one(const std::array<int, N> input, std::array<int, N> expected) {
  assert(!input.empty());
  assert(std::is_heap(input.begin(), input.end()));

  { // (iterator, sentinel) overload.
    auto heapified = input;
    auto b = Iter(heapified.data());
    auto e = Sent(Iter(heapified.data() + heapified.size()));

    auto last = std::ranges::remove_heap(b, e);
    verify_heap(heapified, last, expected);
  }

  { // (range) overload.
    auto heapified = input;
    auto b = Iter(heapified.data());
    auto e = Sent(Iter(heapified.data() + heapified.size()));
    auto range = std::ranges::subrange(b, e);

    auto last = std::ranges::remove_heap(range);
    verify_heap(heapified, last, expected);
  }
}

template <class Iter, class Sent>
constexpr void test_iterators_2() {
  // 1-element sequence.
  test_one<Iter, Sent, 1>({1}, {1});
  // 2-element sequence.
  test_one<Iter, Sent, 2>({2, 1}, {1, 1});
  // 3-element sequence.
  test_one<Iter, Sent, 3>({3, 1, 2}, {2, 1, 2});
  // Longer sequence.
  test_one<Iter, Sent, 8>({11, 8, 5, 6, 4, 3, 2, 1}, {8, 6, 5, 1, 4, 3, 2, 1});
  // Longer sequence with duplicates.
  test_one<Iter, Sent, 8>({8, 8, 6, 6, 1, 2, 2, 3}, {8, 6, 6, 3, 1, 2, 2, 3});
  // All elements are the same.
  test_one<Iter, Sent, 4>({1, 1, 1, 1}, {1, 1, 1, 1});
}

template <class Iter>
constexpr void test_iterators_1() {
  test_iterators_2<Iter, Iter>();
  test_iterators_2<Iter, sentinel_wrapper<Iter>>();
}

constexpr void test_iterators() {
  test_iterators_1<random_access_iterator<int*>>();
  test_iterators_1<contiguous_iterator<int*>>();
  test_iterators_1<int*>();
}

constexpr bool test() {
  test_iterators();

  { // A custom comparator works.
    const std::array input = {1, 2, 3, 5, 4};
    std::array expected = {2, 4, 3, 5, 4};
    auto comp = std::ranges::greater{};
    {
      auto in = input;
      auto last = std::ranges::remove_heap(in.begin(), in.end(), comp);
      assert(last == in.end());
      // Last input element is in unspecified state.
      assert(std::equal(in.begin(), last, expected.begin()));
      assert(std::is_heap(in.begin(), last, comp));
    }

    {
      auto in = input;
      auto last = std::ranges::remove_heap(in, comp);
      assert(last == in.end());
      // Last input element is in unspecified state.
      assert(std::equal(in.begin(), last, expected.begin()));
      assert(std::is_heap(in.begin(), last, comp));
    }
  }

  { // A custom projection works.
    struct A {
      int a;
      constexpr auto operator<=>(const A&) const = default;
    };

    const std::array input = {A{3}, A{1}, A{2}};
    std::array expected = {A{2}, A{1}, A{3}};
    {
      auto in = input;
      auto last = std::ranges::remove_heap(in.begin(), in.end(), {}, &A::a);
      verify_heap(in, last, expected);
    }

    {
      auto in = input;
      auto last = std::ranges::remove_heap(in, {}, &A::a);
      verify_heap(in, last, expected);
    }
  }

  { // `std::invoke` is used in the implementation.
    struct A {
      int i;
      constexpr A(int i_) : i(i_) {}

      constexpr bool comparator(const A& rhs) const { return i < rhs.i; }
      constexpr const A& projection() const { return *this; }

      constexpr auto operator<=>(const A&) const = default;
    };

    const std::array input = {A{3}, A{1}, A{2}};
    std::array expected = {A{2}, A{1}, A{3}};
    {
      auto in = input;
      auto last = std::ranges::remove_heap(in.begin(), in.end(), &A::comparator, &A::projection);
      verify_heap(in, last, expected);
    }

    {
      auto in = input;
      auto last = std::ranges::remove_heap(in, &A::comparator, &A::projection);
      verify_heap(in, last, expected);
    }
  }

  { // `std::ranges::dangling` is returned.
    // [[maybe_unused]] std::same_as<std::ranges::dangling> decltype(auto) result =
    //     std::ranges::remove_heap(std::array{2, 1, 3});
    [[maybe_unused]] decltype(auto) result =
        std::ranges::remove_heap(std::array{1, 3, 2});
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
