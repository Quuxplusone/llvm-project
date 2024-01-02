//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: c++17 || c++20 || c++23
// <optional>

// T shall be an object type

#include <optional>

int main(int, char**)
{
    {
        // expected-error-re@optional:* 2 {{static assertion failed{{.*}}instantiation of optional with a reference type is ill-formed}}
        std::optional<int&> opt1;
        std::optional<int&&> opt2;
    }
    // FIXME these are garbage diagnostics that Clang should not produce
    // expected-error@optional:* 0+ {{is not a base class}}

  return 0;
}
