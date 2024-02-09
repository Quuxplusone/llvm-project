//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <vector>

// Check that a type can be trivial and yet unsafe to trivially relocate
// in vector::reserve. When _LIBCPP_TRIVIALLY_RELOCATABLE_PMR_CONTAINERS,
// we behave non-conformingly.

#include <cassert>
#include <memory>
#include <memory_resource>
#include <type_traits>
#include <vector>

#if defined(_LIBCPP_TRIVIALLY_RELOCATABLE_PMR_CONTAINERS) && _LIBCPP_TRIVIALLY_RELOCATABLE_PMR_CONTAINERS
static constexpr bool PmrIsTrivial = true;
#else
static constexpr bool PmrIsTrivial = false;
#endif

struct Widget {
  using allocator_type = std::pmr::polymorphic_allocator<Widget>;
  explicit Widget(int i) : i_(i) {}
  explicit Widget(int i, allocator_type) : i_(i) {}
  explicit Widget(const Widget& rhs, allocator_type) : i_(rhs.i_ + 100) {}
  int i_;
};
static_assert(std::is_trivially_copyable_v<Widget>);
static_assert(std::uses_allocator_v<Widget, std::pmr::polymorphic_allocator<Widget>>);

int main() {
  {
    std::pmr::vector<Widget> v;
    v.reserve(3);
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);
    assert(v[0].i_ == 1);
    assert(v[1].i_ == 2);
    assert(v[2].i_ == 3);
    v.reserve(v.capacity() + 1);
    assert(v[0].i_ == (PmrIsTrivial ? 1 : 101));
    assert(v[1].i_ == (PmrIsTrivial ? 2 : 102));
    assert(v[2].i_ == (PmrIsTrivial ? 3 : 103));
  }
  return 0;
}
