// Test that adding a new unused decl within reduced BMI may not produce a transitive change.
//
// RUN: rm -rf %t
// RUN: split-file %s %t
//
// RUN: %clang_cc1 -std=c++20 -emit-reduced-module-interface %t/A.cppm -o %t/A.pcm
// RUN: %clang_cc1 -std=c++20 -emit-reduced-module-interface %t/B.cppm -o %t/B.pcm \
// RUN:     -fmodule-file=A=%t/A.pcm
//
// RUN: %clang_cc1 -std=c++20 -emit-reduced-module-interface %t/A.v1.cppm -o %t/A.v1.pcm
// RUN: %clang_cc1 -std=c++20 -emit-reduced-module-interface %t/B.cppm -o %t/B.v1.pcm \
// RUN:     -fmodule-file=A=%t/A.v1.pcm
//
// RUN: not diff %t/B.pcm %t/B.v1.pcm &> /dev/null

//--- A.cppm
export module A;
export int a() { return 44; }

//--- A.v1.cppm
export module A;
export int a() { return 44; }
int a_impl() { return 48; }

//--- B.cppm
export module B;
import A;
export inline int b = a();
