//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// This is a regression test for std::string's allocation behavior when growing
// from the SSO buffer into allocated storage.
#include <cassert>
#include <string>

#include "test_macros.h"

template<class String>
size_t capacity_after_assign_from_c_string_of_length(int n)
{
  String s;
  s = String(n, 'x').c_str();
  return s.capacity();
}

int main(int, char**) {
  assert(capacity_after_assign_from_c_string_of_length<std::string>(1) == 22);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(21) == 22);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(22) == 22);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(23) == 47);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(24) == 47);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(25) == 47);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(30) == 47);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(46) == 47);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(47) == 47);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(48) == 63);
  assert(capacity_after_assign_from_c_string_of_length<std::string>(64) == 79);

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
#  if __WCHAR_WIDTH__ == 32
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(1) == 4);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(3) == 4);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(4) == 4);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(5) == 11);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(6) == 11);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(7) == 11);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(8) == 11);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(9) == 11);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(10) == 11);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(11) == 11);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(12) == 15);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(15) == 15);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(16) == 19);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(20) == 23);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(24) == 27);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(28) == 31);
#  elif __WCHAR_WIDTH__ == 16
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(1) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(3) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(4) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(5) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(6) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(7) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(8) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(9) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(10) == 10);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(11) == 15);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(12) == 15);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(15) == 15);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(16) == 23);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(20) == 23);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(24) == 31);
  assert(capacity_after_assign_from_c_string_of_length<std::wstring>(28) == 31);
#  else
#    error "Unexpected wchar_t width"
#  endif
#endif

  return 0;
}
