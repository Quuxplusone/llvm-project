//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// container_type extract() &&;

#include <deque>
#include <flat_set>
#include <functional>
#include <utility>
#include <vector>

#include "test_macros.h"
#include "min_allocator.h"
#include "Counter.h"
#include "MinSequenceContainer.h"

template<class T>
concept HasExtract = requires {
  { std::declval<T>().extract() };
};

struct MoveSensitiveComp {
  MoveSensitiveComp() noexcept(false) = default;
  MoveSensitiveComp(const MoveSensitiveComp&) noexcept(false) = default;
  MoveSensitiveComp(MoveSensitiveComp&& rhs) noexcept { rhs.is_moved_from_ = true; }
  bool operator()(const auto& a, const auto& b) const { return a < b; }
  bool is_moved_from_ = false;
};

struct ThrowingMoveContainer : MinSequenceContainer<int> {
  using MinSequenceContainer<int>::MinSequenceContainer;
  explicit ThrowingMoveContainer() = default;
  ThrowingMoveContainer(ThrowingMoveContainer&&) { throw 42; }
};

struct CopyOnlyContainer : MinSequenceContainer<int> {
  using MinSequenceContainer<int>::MinSequenceContainer;
  explicit CopyOnlyContainer() = default;
  CopyOnlyContainer(const CopyOnlyContainer& rhs) { *this = rhs; }
  CopyOnlyContainer& operator=(const CopyOnlyContainer&) = default;
};

int main(int, char**)
{
  {
    using M = std::flat_set<int>;
    M m = {1, 5, 4, 9, 2};
    static_assert(!HasExtract<M&>);
    static_assert(!HasExtract<const M&>);
    static_assert(HasExtract<M&&>);
    static_assert(!HasExtract<const M&&>);
    ASSERT_SAME_TYPE(decltype(std::move(m).extract()), std::vector<int>); // returns by value
    std::same_as<std::vector<int>> auto v = std::move(m).extract();
    assert(m.empty());  // "*this is emptied"
    assert((v == std::vector<int>{1, 2, 4, 5, 9}));
  }
  {
    using M = std::flat_set<int, MoveSensitiveComp, std::deque<int, min_allocator<int>>>;
    M m = {1, 3, 6, 10};
    static_assert(!HasExtract<M&>);
    static_assert(!HasExtract<const M&>);
    static_assert(HasExtract<M&&>);
    static_assert(!HasExtract<const M&&>);
    ASSERT_SAME_TYPE(decltype(std::move(m).extract()), M::container_type);
    std::same_as<M::container_type> auto v = std::move(m).extract();
    assert(m.empty());  // "*this is emptied"
    assert(!m.key_comp().is_moved_from_); // comparator remains untouched by extract()
    assert((v == std::deque<int, min_allocator<int>>{1, 3, 6, 10}));
  }
  {
    // Extracting moves (not copies) the container
    using M = std::flat_set<Counter<int>>;
    M m = {1, 2, 3, 4, 5, 6};
    auto old = Counter_base::gConstructed;
    std::same_as<M::container_type> auto v = std::move(m).extract();
    assert(Counter_base::gConstructed == old);
  }
  {
    // *this is emptied (even if the container doesn't clear itself on move).
    CopyOnlyContainer c = {1, 2, 3, 4};
    std::flat_set<int, std::less<>, CopyOnlyContainer> m;
    m.replace(std::move(c));
    assert(c.size() == 4);        // the container type doesn't clear itself,
    (void)std::move(m).extract(); // but when we extract it...
    assert(m.empty());            // ...the flat_set is emptied anyway
  }
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    // *this is emptied, even if the function exits via an exception.
    std::flat_set<int, std::less<>, ThrowingMoveContainer> m = {1, 2, 3, 4};
    try {
      std::move(m).extract();
      assert(false);
    } catch (int ex) {
      assert(ex == 42);
    }
    assert(m.empty());
  }
#endif
  return 0;
}
