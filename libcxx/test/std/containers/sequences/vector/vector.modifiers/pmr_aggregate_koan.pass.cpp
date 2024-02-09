//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <vector>

// Check that std::pmr types are trivially relocated iff they advertise themselves
// as trivially relocatable.

#include <cassert>
#include <memory_resource>
#include <type_traits>
#include <vector>

#if defined(_LIBCPP_TRIVIALLY_RELOCATABLE_PMR_CONTAINERS) && _LIBCPP_TRIVIALLY_RELOCATABLE_PMR_CONTAINERS
static constexpr bool PmrIsTrivial = true;
#else
static constexpr bool PmrIsTrivial = false;
#endif

struct Agg {
  std::pmr::vector<int> pv_;
};
static_assert(std::is_trivially_relocatable_v<Agg> == PmrIsTrivial);

int main() {
  {
    std::pmr::monotonic_buffer_resource mr1;
    std::pmr::monotonic_buffer_resource mr2;
    std::vector<Agg> v;
    v.push_back({ std::pmr::vector<int>({1,2}, &mr1) });
    v.push_back({ std::pmr::vector<int>({3,4}, &mr2) });
    assert(v[0].pv_.get_allocator().resource() == &mr1);
    assert(v[1].pv_.get_allocator().resource() == &mr2);
    v.erase(v.begin());
    if (PmrIsTrivial) {
      assert(v[0].pv_.get_allocator().resource() == &mr2);
    } else {
      assert(v[0].pv_.get_allocator().resource() == &mr1);
    }
  }

  return 0;
}
