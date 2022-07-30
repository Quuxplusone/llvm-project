//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <functional>

#include "test_macros.h"

struct NoRefQuals {
  int operator()() noexcept { return 1; }
  int operator()() const noexcept { return 2; }
};

std::move_only_function<int() noexcept> makeNoRefQuals() {
  return NoRefQuals();
}

std::move_only_function<int() const noexcept> makeNoRefQualsConst() {
  return NoRefQuals();
}

struct RefQuals {
  int operator()() & noexcept { return 3; }
  int operator()() && noexcept { return 4; }
  int operator()() const & noexcept { return 5; }
  int operator()() const && noexcept { return 6; }
};

std::move_only_function<int() & noexcept> makeRefQualsL() {
  return RefQuals();
}
std::move_only_function<int() && noexcept> makeRefQualsR() {
  return RefQuals();
}
std::move_only_function<int() const & noexcept> makeRefQualsConstL() {
  return RefQuals();
}
std::move_only_function<int() const && noexcept> makeRefQualsConstR() {
  return RefQuals();
}

int main(int, char**) {
  {
    std::move_only_function<int()> f = makeNoRefQuals();
    assert(f() == 1);
  }
  {
    std::move_only_function<int() &> f = makeNoRefQuals();
    assert(f() == 1);
  }
  {
    std::move_only_function<int() &&> f = makeNoRefQuals();
    assert(std::move(f)() == 1);
  }
  {
    std::move_only_function<int() noexcept> f = makeNoRefQuals();
    assert(f() == 1);
  }
  {
    std::move_only_function<int() & noexcept> f = makeNoRefQuals();
    assert(f() == 1);
  }
  {
    std::move_only_function<int() && noexcept> f = makeNoRefQuals();
    assert(std::move(f)() == 1);
  }

  {
    std::move_only_function<int()> f = makeNoRefQualsConst();
    assert(f() == 2);
  }
  {
    std::move_only_function<int() &> f = makeNoRefQualsConst();
    assert(f() == 2);
  }
  {
    std::move_only_function<int() &&> f = makeNoRefQualsConst();
    assert(std::move(f)() == 2);
  }
  {
    std::move_only_function<int() const> f = makeNoRefQualsConst();
    assert(f() == 2);
  }
  {
    std::move_only_function<int() const &> f = makeNoRefQualsConst();
    assert(f() == 2);
  }
  {
    std::move_only_function<int() const &&> f = makeNoRefQualsConst();
    assert(std::move(f)() == 2);
  }
  {
    std::move_only_function<int() noexcept> f = makeNoRefQualsConst();
    assert(f() == 2);
  }
  {
    std::move_only_function<int() & noexcept> f = makeNoRefQualsConst();
    assert(f() == 2);
  }
  {
    std::move_only_function<int() && noexcept> f = makeNoRefQualsConst();
    assert(std::move(f)() == 2);
  }
  {
    std::move_only_function<int() const noexcept> f = makeNoRefQualsConst();
    assert(f() == 2);
  }
  {
    std::move_only_function<int() const & noexcept> f = makeNoRefQualsConst();
    assert(f() == 2);
  }
  {
    std::move_only_function<int() const && noexcept> f = makeNoRefQualsConst();
    assert(std::move(f)() == 2);
  }

  static_assert(!std::is_constructible_v<std::move_only_function<int()>, std::move_only_function<int () & noexcept>>);
  {
    std::move_only_function<int() &> f = makeRefQualsL();
    assert(f() == 3);
  }
  static_assert(!std::is_constructible_v<std::move_only_function<int() &&>, std::move_only_function<int () & noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const>, std::move_only_function<int () & noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const &>, std::move_only_function<int () & noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const &&>, std::move_only_function<int () & noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() noexcept>, std::move_only_function<int () & noexcept>>);
  {
    std::move_only_function<int() & noexcept> f = makeRefQualsL();
    assert(f() == 3);
  }
  static_assert(!std::is_constructible_v<std::move_only_function<int() && noexcept>, std::move_only_function<int () & noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const noexcept>, std::move_only_function<int () & noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const & noexcept>, std::move_only_function<int () & noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const && noexcept>, std::move_only_function<int () & noexcept>>);

  static_assert(!std::is_constructible_v<std::move_only_function<int()>, std::move_only_function<int () && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() &>, std::move_only_function<int () && noexcept>>);
  {
    std::move_only_function<int() &&> f = makeRefQualsR();
    assert(std::move(f)() == 4);
  }
  static_assert(!std::is_constructible_v<std::move_only_function<int() const>, std::move_only_function<int () && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const &>, std::move_only_function<int () && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const &&>, std::move_only_function<int () && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() noexcept>, std::move_only_function<int () && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() & noexcept>, std::move_only_function<int () && noexcept>>);
  {
    std::move_only_function<int() && noexcept> f = makeRefQualsR();
    assert(std::move(f)() == 4);
  }
  static_assert(!std::is_constructible_v<std::move_only_function<int() const noexcept>, std::move_only_function<int () && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const & noexcept>, std::move_only_function<int () && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const && noexcept>, std::move_only_function<int () && noexcept>>);

  {
    std::move_only_function<int()> f = makeRefQualsConstL();
    assert(f() == 5);
  }
  {
    std::move_only_function<int() &> f = makeRefQualsConstL();
    assert(f() == 5);
  }
  {
    std::move_only_function<int() &&> f = makeRefQualsConstL();
    assert(std::move(f)() == 5);
  }
  {
    std::move_only_function<int() const> f = makeRefQualsConstL();
    assert(f() == 5);
  }
  {
    std::move_only_function<int() const &> f = makeRefQualsConstL();
    assert(f() == 5);
  }
  {
    std::move_only_function<int() const &&> f = makeRefQualsConstL();
    assert(std::move(f)() == 5);
  }
  {
    std::move_only_function<int() noexcept> f = makeRefQualsConstL();
    assert(f() == 5);
  }
  {
    std::move_only_function<int() & noexcept> f = makeRefQualsConstL();
    assert(f() == 5);
  }
  {
    std::move_only_function<int() && noexcept> f = makeRefQualsConstL();
    assert(std::move(f)() == 5);
  }
  {
    std::move_only_function<int() const noexcept> f = makeRefQualsConstL();
    assert(f() == 5);
  }
  {
    std::move_only_function<int() const & noexcept> f = makeRefQualsConstL();
    assert(f() == 5);
  }
  {
    std::move_only_function<int() const && noexcept> f = makeRefQualsConstL();
    assert(std::move(f)() == 5);
  }

  static_assert(!std::is_constructible_v<std::move_only_function<int()>, std::move_only_function<int () const && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() &>, std::move_only_function<int () const && noexcept>>);
  {
    std::move_only_function<int() &&> f = makeRefQualsConstR();
    assert(std::move(f)() == 6);
  }
  static_assert(!std::is_constructible_v<std::move_only_function<int() const>, std::move_only_function<int () const && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const &>, std::move_only_function<int () const && noexcept>>);
  {
    std::move_only_function<int() const &&> f = makeRefQualsConstR();
    assert(std::move(f)() == 6);
  }
  static_assert(!std::is_constructible_v<std::move_only_function<int() noexcept>, std::move_only_function<int () const && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() & noexcept>, std::move_only_function<int () const && noexcept>>);
  {
    std::move_only_function<int() && noexcept> f = makeRefQualsConstR();
    assert(std::move(f)() == 6);
  }
  static_assert(!std::is_constructible_v<std::move_only_function<int() const noexcept>, std::move_only_function<int () const && noexcept>>);
  static_assert(!std::is_constructible_v<std::move_only_function<int() const & noexcept>, std::move_only_function<int () const && noexcept>>);
  {
    std::move_only_function<int() const && noexcept> f = makeRefQualsConstR();
    assert(std::move(f)() == 6);
  }
  return 0;
}
