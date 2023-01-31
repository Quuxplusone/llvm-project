//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <hive>

// bool empty() const noexcept;

#include <hive>

void test() {
    std::hive<int> c;
    c.empty(); // expected-warning {{ignoring return value of function declared with 'nodiscard' attribute}}
}
