//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<forward_iterator I, sentinel_for<I> S, class Proj = identity,
//          indirect_binary_predicate<projected<I, Proj>,
//                                    projected<I, Proj>> Pred = ranges::equal_to>
//   constexpr bool ranges::is_uniqued(I first, S last, Pred pred = {}, Proj proj = {});
// template<forward_range R, class Proj = identity,
//          indirect_binary_predicate<projected<iterator_t<R>, Proj>,
//                                    projected<iterator_t<R>, Proj>> Pred = ranges::equal_to>
//   constexpr bool ranges::is_uniqued(R&& r, Pred pred = {}, Proj proj = {});

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <ranges>

#include "almost_satisfies_types.h"
#include "boolean_testable.h"
#include "test_iterators.h"

template <class Iter, class Sent = Iter>
concept HasIsUniquedIt = requires (Iter iter, Sent sent) { std::ranges::is_uniqued(iter, sent); };

struct NotComparable {};

static_assert(HasIsUniquedIt<int*>);
static_assert(!HasIsUniquedIt<ForwardIteratorNotDerivedFrom>);
static_assert(!HasIsUniquedIt<ForwardIteratorNotIncrementable>);
static_assert(!HasIsUniquedIt<int*, SentinelForNotSemiregular>);
static_assert(!HasIsUniquedIt<int*, SentinelForNotWeaklyEqualityComparableWith>);
static_assert(!HasIsUniquedIt<NotComparable*>);

template <class Range>
concept HasIsUniquedR = requires (Range range) { std::ranges::is_uniqued(range); };

static_assert(HasIsUniquedR<UncheckedRange<int*>>);
static_assert(!HasIsUniquedR<ForwardRangeNotDerivedFrom>);
static_assert(!HasIsUniquedR<ForwardRangeNotIncrementable>);
static_assert(!HasIsUniquedR<ForwardRangeNotSentinelSemiregular>);
static_assert(!HasIsUniquedR<ForwardRangeNotSentinelEqualityComparableWith>);
static_assert(!HasIsUniquedR<UncheckedRange<NotComparable>>);

template <std::size_t N>
struct Data {
  std::array<int, N> input;
  bool expected;
};

template <class Iter, class Sent, std::size_t N>
constexpr void test(Data<N> d) {
  {
    std::same_as<bool> decltype(auto) ret =
        std::ranges::is_uniqued(Iter(d.input.data()), Sent(Iter(d.input.data() + d.input.size())));
    assert(ret == d.expected);
  }
  {
    auto range = std::ranges::subrange(Iter(d.input.data()), Sent(Iter(d.input.data() + d.input.size())));
    std::same_as<bool> decltype(auto) ret = std::ranges::is_uniqued(range);
    assert(base(ret) == d.expected);
  }
}

template <class Iter, class Sent = Iter>
constexpr void test_iterators() {
  // simple test
  test<Iter, Sent, 4>({.input = {1, 2, 2, 4}, .expected = false});
  // last is returned with no match
  test<Iter, Sent, 4>({.input = {1, 2, 3, 4}, .expected = true});
  // first elements match
  test<Iter, Sent, 4>({.input = {1, 1, 3, 4}, .expected = false});
  // the first match is returned
  test<Iter, Sent, 7>({.input = {1, 1, 3, 4, 4, 4, 4}, .expected = false});
  // two element range works
  test<Iter, Sent, 2>({.input = {3, 3}, .expected = false});
  // single element range works
  test<Iter, Sent, 1>({.input = {1}, .expected = true});
  // empty range works
  test<Iter, Sent, 0>({.input = {}, .expected = true});
}

constexpr bool test() {
  test_iterators<forward_iterator<int*>, sentinel_wrapper<forward_iterator<int*>>>();
  test_iterators<forward_iterator<int*>>();
  test_iterators<bidirectional_iterator<int*>>();
  test_iterators<random_access_iterator<int*>>();
  test_iterators<contiguous_iterator<int*>>();
  test_iterators<int*>();
  test_iterators<const int*>();

  { // check an rvalue range
    std::same_as<bool> decltype(auto) ret =
        std::ranges::is_uniqued(std::array{1, 2, 3, 4});
    assert(ret == true);
  }

  { // check that the complexity requirements are met with no match
    {
      int predicateCount = 0;
      auto pred = [&](int, int) { ++predicateCount; return false; };
      auto projectionCount = 0;
      auto proj = [&](int i) { ++projectionCount; return i; };
      int a[] = {1, 2, 3, 4, 5};
      auto ret = std::ranges::is_uniqued(a, a + 5, pred, proj);
      assert(ret == true);
      assert(predicateCount == 4);
      assert(projectionCount == 8);
    }
    {
      int predicateCount = 0;
      auto pred = [&](int, int) { ++predicateCount; return false; };
      auto projectionCount = 0;
      auto proj = [&](int i) { ++projectionCount; return i; };
      int a[] = {1, 2, 3, 4, 5};
      auto ret = std::ranges::is_uniqued(a, pred, proj);
      assert(ret == true);
      assert(predicateCount == 4);
      assert(projectionCount == 8);
    }
  }

  { // check that the complexity requirements are met with a match
    {
      int predicateCount = 0;
      auto pred = [&](int i, int j) { ++predicateCount; return i == j; };
      auto projectionCount = 0;
      auto proj = [&](int i) { ++projectionCount; return i; };
      int a[] = {1, 2, 4, 4, 5};
      auto ret = std::ranges::is_uniqued(a, a + 5, pred, proj);
      assert(ret == false);
      assert(predicateCount == 3);
      assert(projectionCount == 6);
    }
    {
      int predicateCount = 0;
      auto pred = [&](int i, int j) { ++predicateCount; return i == j; };
      auto projectionCount = 0;
      auto proj = [&](int i) { ++projectionCount; return i; };
      int a[] = {1, 2, 4, 4, 5};
      auto ret = std::ranges::is_uniqued(a, pred, proj);
      assert(ret == false);
      assert(predicateCount == 3);
      assert(projectionCount == 6);
    }
  }

  { // check that std::invoke is used
    struct S {
      constexpr S(int i_) : i(i_) {}
      constexpr bool compare(const S& j) const { return j.i == i; }
      constexpr const S& identity() const { return *this; }
      int i;
    };
    {
      S a[] = {1, 2, 3, 4};
      auto ret = std::ranges::is_uniqued(std::begin(a), std::end(a), &S::compare, &S::identity);
      assert(ret == true);
    }
    {
      S a[] = {1, 2, 3, 4};
      auto ret = std::ranges::is_uniqued(a, &S::compare, &S::identity);
      assert(ret == true);
    }
  }

  { // check that the implicit conversion to bool works
    {
      int a[] = {1, 2, 2, 4};
      auto ret = std::ranges::is_uniqued(a, a + 4, [](int i, int j) { return BooleanTestable{i == j}; });
      assert(ret == false);
    }
    {
      int a[] = {1, 2, 2, 4};
      auto ret = std::ranges::is_uniqued(a, [](int i, int j) { return BooleanTestable{i == j}; });
      assert(ret == false);
    }
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;
}
