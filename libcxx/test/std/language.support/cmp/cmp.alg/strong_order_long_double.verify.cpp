//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// The following platforms have sizeof(long double) == sizeof(double), so this test doesn't apply to them.
// This test does apply to aarch64 where Arm's AAPCS64 is followed. There they are different sizes.
// UNSUPPORTED: target={{arm64|armv(7|8)(l|m)?|powerpc|powerpc64}}-{{.+}}
// MSVC configurations have long double equal to regular double on all
// architectures.
// UNSUPPORTED: target={{.+}}-pc-windows-msvc
// ARM/AArch64 MinGW also has got long double equal to regular double, just
// like MSVC (thus match both MinGW and MSVC here, for those architectures).
// UNSUPPORTED: target={{aarch64|armv7}}-{{.*}}-windows-{{.+}}
// Android's 32-bit x86 target has long double equal to regular double.
// UNSUPPORTED: target=i686-{{.+}}-android{{.*}}

// <compare>

// template<class T> constexpr strong_ordering strong_order(const T& a, const T& b);

// libc++ does not support strong_order(long double, long double) quite yet.
// This test verifies the error message we give for that case.
// TODO: remove this test once long double is properly supported.

#include <compare>

void f() {
    long double ld = 3.14;
    (void)std::strong_order(ld, ld);
        // expected-error@-1 {{no matching function for call}}
        // expected-note@*:* {{candidate template ignored: constraints not satisfied}}
        // expected-note@*:* {{because 'long double &' does not satisfy '__strong_order_for_long_double_is_not_yet_implemented'}}
        // expected-note@*:* {{because '!is_same_v<decay_t<long double &>, long double>' evaluated to false}}
}
