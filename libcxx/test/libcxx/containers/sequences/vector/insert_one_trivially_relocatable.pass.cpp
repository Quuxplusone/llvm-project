//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// iterator insert(const_iterator it, const value_type& x);
// iterator insert(const_iterator it, value_type&& x);

#include <vector>
#include <cassert>

static int countdown = 0;
static int alive = 0;
static int assignments = 0;

template<bool TR>
struct _LIBCPP_TRIVIALLY_RELOCATABLE_IF(TR) MyClass {
  int value;
  explicit MyClass(int i) : value(i) { alive += 1; }
  MyClass(const MyClass& rhs) {
    if (--countdown == 0) throw 42;
    value = rhs.value;
    alive += 1;
  }
  MyClass& operator=(const MyClass& rhs) {
    assignments += 1;
    value = rhs.value;
    return *this;
  }
  ~MyClass() { alive -= 1; }
};
static_assert(std::is_trivially_relocatable<MyClass<true> >::value, "");
static_assert(!std::is_trivially_relocatable<MyClass<false> >::value, "");

template<bool TR>
void test()
{
  alive = 0;

  using T = MyClass<TR>;
  using It = typename std::vector<T>::iterator;
  std::vector<T> vec;
  vec.reserve(100);
  vec.push_back(T(1));
  vec.push_back(T(2));
  vec.push_back(T(3));
  assert(alive == 3);

  {
    assignments = 0;
    It it = vec.insert(vec.begin() + 2, T(4));
    assert(it == vec.begin() + 2);
    if (TR) {
      assert(assignments == 0);  // relocate upward without assigning
    } else {
      assert(assignments >= 1);
    }
  }
  assert(vec.size() == 4);
  assert(alive == 4);

  {
    assignments = 0;
    T lvalue = T(5);
    It it = vec.insert(vec.begin() + 2, lvalue);
    assert(it == vec.begin() + 2);
    if (TR) {
      assert(assignments == 0);  // relocate upward without assigning
    } else {
      assert(assignments >= 1);
    }
  }
  assert(vec.size() == 5);
  assert(alive == 5);
  assert(vec[0].value == 1);
  assert(vec[1].value == 2);
  assert(vec[2].value == 5);
  assert(vec[3].value == 4);
  assert(vec[4].value == 3);

  {
    // Test exception-safety.
    for (int n = 1; true; ++n) {
      try {
        countdown = n;
        vec.insert(vec.begin(), T(6));
        break;
      } catch (int x) {
        assert(x == 42);
        assert(countdown == 0); // attempted no throwing ops after the first throw
        assert(alive == vec.size());
        assert(vec.size() == 5);
      }
    }
    countdown = 0;
  }
  assert(vec.size() == 6);
  assert(alive == 6);

  vec.clear();
  assert(alive == 0);
}

int main(int, char**)
{
  test<true>();
  test<false>();

  return 0;
}
