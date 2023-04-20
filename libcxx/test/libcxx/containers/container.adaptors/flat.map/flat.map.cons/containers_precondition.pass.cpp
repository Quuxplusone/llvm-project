//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: has-unix-headers
// UNSUPPORTED: !libcpp-has-debug-mode
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <flat_map>

// test precondition of flat_map(ks, vs) constructors

#include "check_assertion.h"

std::vector<int> rvalue_ks() {
  return {1,2,3};
}

std::vector<int> rvalue_vs() {
  return {1,2};
}

int main(int, char**) {
  std::vector<int> ks = rvalue_ks();
  std::vector<int> vs = rvalue_vs();
  using C = std::less<>;
  using A = std::allocator<int>;

  // The CTAD here is gratuitous, but we might as well exercise these deduction guides.
  {
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(ks, vs), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(ks, rvalue_vs()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(rvalue_ks(), vs), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(rvalue_ks(), rvalue_vs()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(ks, vs, C()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(ks, rvalue_vs(), C()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(rvalue_ks(), vs, C()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(rvalue_ks(), rvalue_vs(), C()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(ks, vs, A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(ks, rvalue_vs(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(rvalue_ks(), vs, A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(rvalue_ks(), rvalue_vs(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(ks, vs, C(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(ks, rvalue_vs(), C(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(rvalue_ks(), vs, C(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(rvalue_ks(), rvalue_vs(), C(), A()), "key_cont and mapped_cont must be equal in size");
  }
  {
    auto s = std::sorted_unique;
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, ks, vs), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, ks, rvalue_vs()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, rvalue_ks(), vs), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, rvalue_ks(), rvalue_vs()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, ks, vs, C()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, ks, rvalue_vs(), C()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, rvalue_ks(), vs, C()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, rvalue_ks(), rvalue_vs(), C()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, ks, vs, A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, ks, rvalue_vs(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, rvalue_ks(), vs, A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, rvalue_ks(), rvalue_vs(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, ks, vs, C(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, ks, rvalue_vs(), C(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, rvalue_ks(), vs, C(), A()), "key_cont and mapped_cont must be equal in size");
    TEST_LIBCPP_ASSERT_FAILURE(std::flat_map(s, rvalue_ks(), rvalue_vs(), C(), A()), "key_cont and mapped_cont must be equal in size");
  }
  return 0;
}
