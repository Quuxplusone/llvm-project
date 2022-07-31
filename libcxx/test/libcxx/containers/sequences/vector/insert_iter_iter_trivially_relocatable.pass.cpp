//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// iterator insert(const_iterator it, InputIterator first, InputIterator last);

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
    // From 1,2,3 to 1,4,5,2,3 -- 2,3 is relocated upward exactly-without-overlap
    assignments = 0;
    T a[2] = { T(4), T(5) };
    It it = vec.insert(vec.begin() + 1, a, a + 2);
    assert(it == vec.begin() + 1);
    if (TR) {
      assert(assignments == 0);  // relocate upward without assigning
    } else {
      assert(assignments >= 2);
    }
  }
  assert(vec.size() == 5);
  assert(alive == 5);

  {
    // From 1,4,5,2,3 to 1,4,6,7,5,2,3 -- 5,2,3 is relocated upward with overlap
    assignments = 0;
    T a[2] = { T(6), T(7) };
    It it = vec.insert(vec.begin() + 2, a, a + 2);
    assert(it == vec.begin() + 2);
    if (TR) {
      assert(assignments == 0);  // relocate upward without assigning
    } else {
      assert(assignments >= 3);
    }
  }
  assert(vec.size() == 7);
  assert(alive == 7);

  {
    // From 1,4,6,7,5,2,3 to 1,4,6,7,5,8,9,10,11,2,3 -- 2,3 is relocated upward well-beyond-overlap
    assignments = 0;
    T a[4] = { T(8), T(9), T(10), T(11) };
    It it = vec.insert(vec.begin() + 5, a, a + 4);
    assert(it == vec.begin() + 5);
    if (TR) {
      assert(assignments == 0);  // relocate upward without assigning
    } else {
      assert(assignments >= 2);
    }
  }
  assert(vec.size() == 11);
  assert(alive == 11);
  assert(vec[0].value == 1);
  assert(vec[1].value == 4);
  assert(vec[2].value == 6);
  assert(vec[3].value == 7);
  assert(vec[4].value == 5);
  assert(vec[5].value == 8);
  assert(vec[6].value == 9);
  assert(vec[7].value == 10);
  assert(vec[8].value == 11);
  assert(vec[9].value == 2);
  assert(vec[10].value == 3);
}

template<bool TR>
void test_exception_safety()
{
  using T = MyClass<TR>;
  using It = typename std::vector<T>::iterator;
  T a[] = { T(6), T(7), T(8), T(9) };

  alive = 0;
  {
    for (int n = 1; true; ++n) {
      countdown = 0;
      std::vector<T> vec = { T(1), T(2), T(3), T(4), T(5) };
      vec.reserve(100);
      assert(alive == 5);
      try {
        countdown = n;
        vec.insert(vec.begin() + 2, a, a + 2);
        assert(vec.size() == 7);
        assert(alive == 7);
        assert(vec[0].value == 1);
        assert(vec[1].value == 2);
        assert(vec[2].value == 6);
        assert(vec[3].value == 7);
        assert(vec[4].value == 3);
        assert(vec[5].value == 4);
        assert(vec[6].value == 5);
        break;
      } catch (int x) {
        assert(x == 42);
        assert(countdown == 0); // attempted no throwing ops after the first throw
        assert(alive == vec.size());
        assert(5 <= vec.size());
        assert(vec.size() <= 7);
        assert(vec[0].value == 1);
        assert(vec[1].value == 2);
        assert(vec[2].value == 3 || vec[2].value == 6);
        if (TR) {
          assert(vec[vec.size()-3].value == 3);
          assert(vec[vec.size()-2].value == 4);
          assert(vec[vec.size()-1].value == 5);
        }
      }
    }
  }

  assert(alive == 0);
  {
    for (int n = 1; true; ++n) {
      countdown = 0;
      std::vector<T> vec = { T(1), T(2), T(3), T(4), T(5) };
      vec.reserve(100);
      assert(alive == 5);
      try {
        countdown = n;
        vec.insert(vec.begin() + 2, a, a + 3);
        assert(vec.size() == 8);
        assert(alive == 8);
        assert(vec[0].value == 1);
        assert(vec[1].value == 2);
        assert(vec[2].value == 6);
        assert(vec[3].value == 7);
        assert(vec[4].value == 8);
        assert(vec[5].value == 3);
        assert(vec[6].value == 4);
        assert(vec[7].value == 5);
        break;
      } catch (int x) {
        assert(x == 42);
        assert(countdown == 0); // attempted no throwing ops after the first throw
        assert(alive == vec.size());
        assert(5 <= vec.size());
        assert(vec.size() <= 8);
        assert(vec[0].value == 1);
        assert(vec[1].value == 2);
        assert(vec[2].value == 3 || vec[2].value == 6);
        if (TR) {
          assert(vec[vec.size()-3].value == 3);
          assert(vec[vec.size()-2].value == 4);
          assert(vec[vec.size()-1].value == 5);
        }
      }
    }
  }

  assert(alive == 0);
  {
    for (int n = 1; true; ++n) {
      countdown = 0;
      std::vector<T> vec = { T(1), T(2), T(3), T(4), T(5) };
      vec.reserve(100);
      assert(alive == 5);
      try {
        countdown = n;
        vec.insert(vec.begin() + 2, a, a + 4);
        assert(vec.size() == 9);
        assert(alive == 9);
        assert(vec[0].value == 1);
        assert(vec[1].value == 2);
        assert(vec[2].value == 6);
        assert(vec[3].value == 7);
        assert(vec[4].value == 8);
        assert(vec[5].value == 9);
        assert(vec[6].value == 3);
        assert(vec[7].value == 4);
        assert(vec[8].value == 5);
        break;
      } catch (int x) {
        assert(x == 42);
        assert(countdown == 0); // attempted no throwing ops after the first throw
        assert(alive == vec.size());
        assert(5 <= vec.size());
        assert(vec.size() <= 9);
        assert(vec[0].value == 1);
        assert(vec[1].value == 2);
        assert(vec[2].value == 3 || vec[2].value == 6 || vec[2].value == 9);
        if (TR) {
          assert(vec[vec.size()-3].value == 3);
          assert(vec[vec.size()-2].value == 4);
          assert(vec[vec.size()-1].value == 5);
        }
      }
    }
  }

  assert(alive == 0);
}

int main(int, char**)
{
  test<true>();
  test<false>();

  test_exception_safety<true>();
  test_exception_safety<false>();

  return 0;
}
