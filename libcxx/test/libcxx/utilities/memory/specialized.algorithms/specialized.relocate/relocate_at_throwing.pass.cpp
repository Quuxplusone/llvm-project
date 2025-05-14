//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03
// UNSUPPORTED: no-exceptions

// <memory>

// template <class T>
// T *relocate_at(T *source, T *dest);

#include <memory>
#include <cstdlib>
#include <cassert>

struct Counted {
  static int count;
  int m_data;
  Counted(int i) : m_data(i) { ++count; }
  Counted(Counted&&) { throw 42; }
  ~Counted() { --count; }
  friend void operator&(Counted) = delete;
};
int Counted::count = 0;

struct [[clang::trivially_relocatable]] RCounted {
  static int count;
  int m_data;
  RCounted(int i) : m_data(i) { ++count; }
  RCounted(RCounted&&) { throw 42; }
  ~RCounted() { --count; }
  friend void operator&(RCounted) = delete;
};
int RCounted::count = 0;

int main(int, char**)
{
  {
    void *mem1 = std::malloc(sizeof(Counted));
    void *mem2 = std::malloc(sizeof(Counted));
    assert(mem1 && mem2);
    assert(Counted::count == 0);
    Counted *ptr1 = ::new (mem1) Counted(1234);
    Counted *ptr2 = static_cast<Counted*>(mem2);
    assert(Counted::count == 1);
    try {
      std::relocate_at(ptr1, ptr2);
      assert(false);  // the move-ctor should have thrown
    } catch (int x) {
      assert(x == 42);
    }
    assert(Counted::count == 0);
    std::free(mem1);
    std::free(mem2);
  }
  {
    void *mem1 = std::malloc(sizeof(RCounted));
    void *mem2 = std::malloc(sizeof(RCounted));
    assert(mem1 && mem2);
    assert(RCounted::count == 0);
    RCounted *ptr1 = ::new (mem1) RCounted(5678);
    RCounted *ptr2 = static_cast<RCounted*>(mem2);
    assert(RCounted::count == 1);
    std::relocate_at(ptr1, ptr2);  // the move-ctor should not have been called
    assert(RCounted::count == 1);
    assert(ptr2->m_data == 5678);
    ptr2->~RCounted();
    std::free(mem1);
    std::free(mem2);
  }
  return 0;
}
