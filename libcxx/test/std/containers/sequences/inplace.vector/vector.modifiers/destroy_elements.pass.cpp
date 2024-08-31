//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20, c++23

// <inplace_vector>

// Ensure that all the elements in the inplace_vector are destroyed
// Note they are not destroyed for trivial types in a constant expression,
// Clang bug makes this not possible: https://github.com/llvm/llvm-project/issues/105932
// And it isn't possible to have a constexpr variable if they are destroyed

#include <algorithm>
#include <array>
#include <cassert>
#include <inplace_vector>
#include <vector>

#include "test_macros.h"

namespace my {
  template<class T>
  using vector = std::inplace_vector<T, 1000>;
} // namespace my

struct DestroyTracker {
  constexpr DestroyTracker(std::vector<bool>& vec) : vec_(&vec), index_(vec.size()) { vec.push_back(false); }

  constexpr DestroyTracker(const DestroyTracker& other) : vec_(other.vec_), index_(vec_->size()) {
    vec_->push_back(false);
  }

  constexpr DestroyTracker& operator=(const DestroyTracker&) { return *this; }
  constexpr ~DestroyTracker() { (*vec_)[index_] = true; }

  std::vector<bool>* vec_;
  size_t index_;
};

template <class Operation>
TEST_CONSTEXPR_CXX20 void test(Operation operation) {
  std::vector<bool> all_destroyed;

  {
    my::vector<DestroyTracker> v;
    for (size_t i = 0; i != 100; ++i)
      operation(v, all_destroyed);
  }

  assert(std::all_of(all_destroyed.begin(), all_destroyed.end(), [](bool b) { return b; }));
}

constexpr bool test() {
  test([](my::vector<DestroyTracker>& vec, std::vector<bool>& tracker) { vec.emplace_back(tracker); });
  test([](my::vector<DestroyTracker>& vec, std::vector<bool>& tracker) { vec.push_back(tracker); });
  test([](my::vector<DestroyTracker>& vec, std::vector<bool>& tracker) { vec.emplace(vec.begin(), tracker); });
  test([](my::vector<DestroyTracker>& vec, std::vector<bool>& tracker) { vec.insert(vec.begin(), tracker); });
  test([](my::vector<DestroyTracker>& vec, std::vector<bool>& tracker) { vec.resize(vec.size() + 1, tracker); });
  test([](my::vector<DestroyTracker>& vec, std::vector<bool>& tracker) {
    vec.insert_range(vec.begin(), std::array<DestroyTracker, 2>{tracker, tracker});
  });
  test([](my::vector<DestroyTracker>& vec, std::vector<bool>& tracker) {
    vec.append_range(std::array<DestroyTracker, 2>{tracker, tracker});
  });

  return true;
}

int main() {
  test();
#if TEST_STD_VER >= 20
  static_assert(test());
#endif
}
