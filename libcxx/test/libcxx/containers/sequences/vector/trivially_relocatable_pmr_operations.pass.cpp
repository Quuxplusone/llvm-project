//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <vector>

// Reallocating, inserting, and erasing in a pmr::vector will use memcpy
// when it can. This is controlled by _LIBCPP_TRIVIALLY_RELOCATABLE_PMR_CONTAINERS.

#include <cassert>
#include <memory>
#include <memory_resource>
#include <string>
#include <vector>

static constexpr bool PmrIsTrivial =
#if defined(_LIBCPP_TRIVIALLY_RELOCATABLE_PMR_CONTAINERS) && _LIBCPP_TRIVIALLY_RELOCATABLE_PMR_CONTAINERS
  true;
#else
  false;
#endif

void break_pmr_invariant(std::pmr::vector<std::pmr::string>& v, std::pmr::monotonic_buffer_resource *mrs)
{
  using T = std::pmr::string;
  int n = v.size();
  std::destroy(v.begin(), v.end());
  for (int i = 0; i < n; ++i) {
    ::new (&v[i]) T(std::to_string(i), &mrs[i]);
  }
}

void test_reallocate()
{
  std::pmr::monotonic_buffer_resource mr[4];
  std::pmr::vector<std::pmr::string> v(3, &mr[3]);
  break_pmr_invariant(v, mr);

  v.reserve(v.capacity() + 10);
  assert(v.size() == 3);
  assert((v == decltype(v){ "0", "1", "2" }));
  assert(v[0].get_allocator().resource() == (PmrIsTrivial ? &mr[3] : &mr[3]));
  assert(v[1].get_allocator().resource() == (PmrIsTrivial ? &mr[3] : &mr[3]));
  assert(v[2].get_allocator().resource() == (PmrIsTrivial ? &mr[3] : &mr[3]));
}

void test_insert_one()
{
  std::pmr::monotonic_buffer_resource mr[6];
  std::pmr::vector<std::pmr::string> v(5, &mr[5]);
  v.reserve(6);
  break_pmr_invariant(v, mr);

  v.insert(v.begin() + 2, "a");
  assert(v.size() == 6);
  assert((v == decltype(v){ "0", "1", "a", "2", "3", "4" }));
  assert(v[0].get_allocator().resource() == &mr[0]);
  assert(v[1].get_allocator().resource() == &mr[1]);
  assert(v[2].get_allocator().resource() == (PmrIsTrivial ? &mr[2] : &mr[2]));
  assert(v[3].get_allocator().resource() == (PmrIsTrivial ? &mr[3] : &mr[3]));
  assert(v[4].get_allocator().resource() == (PmrIsTrivial ? &mr[4] : &mr[4]));
  assert(v[5].get_allocator().resource() == (PmrIsTrivial ? &mr[5] : &mr[5]));
}

void test_insert_range()
{
  std::pmr::monotonic_buffer_resource mr[8];
  using T = std::pmr::string;
  std::pmr::vector<T> v(5, &mr[7]);
  v.reserve(7);
  break_pmr_invariant(v, mr);

  T a[] = {
    T("a", &mr[5]),
    T("b", &mr[6]),
  };
  v.insert(v.begin() + 2, a, a + 2);
  assert(v.size() == 7);
  assert((v == decltype(v){ "0", "1", "a", "b", "2", "3", "4" }));
  assert(v[0].get_allocator().resource() == &mr[0]);
  assert(v[1].get_allocator().resource() == &mr[1]);
  assert(v[2].get_allocator().resource() == (PmrIsTrivial ? &mr[2] : &mr[2]));
  assert(v[3].get_allocator().resource() == (PmrIsTrivial ? &mr[3] : &mr[3]));
  assert(v[4].get_allocator().resource() == (PmrIsTrivial ? &mr[4] : &mr[4]));
  assert(v[5].get_allocator().resource() == (PmrIsTrivial ? &mr[7] : &mr[7]));
  assert(v[6].get_allocator().resource() == (PmrIsTrivial ? &mr[7] : &mr[7]));
}

void test_erase_one()
{
  std::pmr::monotonic_buffer_resource mr[5];
  std::pmr::vector<std::pmr::string> v(5);
  break_pmr_invariant(v, mr);

  v.erase(v.begin() + 2);
  assert(v.size() == 4);
  assert((v == decltype(v){ "0", "1", "3", "4" }));
  assert(v[0].get_allocator().resource() == &mr[0]);
  assert(v[1].get_allocator().resource() == &mr[1]);
  assert(v[2].get_allocator().resource() == (PmrIsTrivial ? &mr[2] : &mr[2]));
  assert(v[3].get_allocator().resource() == (PmrIsTrivial ? &mr[3] : &mr[3]));
}

void test_erase_range()
{
  std::pmr::monotonic_buffer_resource mr[5];
  std::pmr::vector<std::pmr::string> v(5);
  break_pmr_invariant(v, mr);

  v.erase(v.begin() + 1, v.begin() + 3);
  assert(v.size() == 3);
  assert((v == decltype(v){ "0", "3", "4" }));
  assert(v[0].get_allocator().resource() == &mr[0]);
  assert(v[1].get_allocator().resource() == (PmrIsTrivial ? &mr[1] : &mr[1]));
  assert(v[2].get_allocator().resource() == (PmrIsTrivial ? &mr[2] : &mr[2]));
}

int main(int, char**)
{
    test_reallocate();
    test_insert_one();
    test_insert_range();
    test_erase_one();
    test_erase_range();

    return 0;
}
