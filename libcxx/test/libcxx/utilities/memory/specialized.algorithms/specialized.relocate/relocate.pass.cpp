//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <memory>

// template <class _Tp>
// void relocate(_Tp *source, _Tp *dest);

#include <memory>
#include <cstdlib>
#include <cassert>

struct Counted {
  static int count;
  int m_data;
  Counted(int i) : m_data(i) { ++count; }
  Counted(Counted&& rhs) : m_data(rhs.m_data) { rhs.m_data = 0; ++count; }
  ~Counted() { --count; }
  friend void operator&(Counted) = delete;
};
int Counted::count = 0;

#ifdef _LIBCPP_TRIVIALLY_RELOCATABLE
struct _LIBCPP_TRIVIALLY_RELOCATABLE RCounted {
#else
struct RCounted {
#endif
  static int count;
  int m_data;
  RCounted(int i) : m_data(i) { ++count; }
  RCounted(RCounted&& rhs) : m_data(rhs.m_data) { ++count; }
  ~RCounted() { --count; }
  friend void operator&(RCounted) = delete;
};
int RCounted::count = 0;

int main(int, char**)
{
    {
    void *mem1 = std::malloc(sizeof(Counted));
    assert(mem1);
    assert(Counted::count == 0);
    Counted *ptr1 = ::new (mem1) Counted(1234);
    assert(Counted::count == 1);
    Counted var2 = std::relocate(ptr1);
    assert(Counted::count == 1);
    assert(var2.m_data == 1234);
    std::free(mem1);
    }

    {
    void *mem1 = std::malloc(sizeof(RCounted));
    assert(mem1);
    assert(RCounted::count == 0);
    RCounted *ptr1 = ::new (mem1) RCounted(5678);
    assert(RCounted::count == 1);
    RCounted var2 = std::relocate(ptr1);
    assert(RCounted::count == 1);
    assert(var2.m_data == 5678);
    std::free(mem1);
    }

    return 0;
}
