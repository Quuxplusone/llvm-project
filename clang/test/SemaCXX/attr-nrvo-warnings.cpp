// RUN: %clang_cc1 -verify -disable-llvm-passes -emit-llvm -o - -triple x86_64-linux %s

// This file must contain only warnings, because if it contained an error,
// then no code would be generated, and then we wouldn't get the warnings
// from the CodeGen pass which are the whole point of this file.

struct S { S(); S(S&&); ~S(); };

S ReturnsWithNRVO() {
  S x;
  [[clang::nrvo]] return x; // OK
}

S ReturnsWithNRVOAndConditional(bool cond) {
  if (cond) {
    S x;
    [[clang::nrvo]] return x; // OK
  } else {
    S y;
    [[clang::nrvo]] return (y); // OK
  }
}

S ReturnsWithoutNRVO(bool cond) {
  S x;
  if (cond) {
    S y;
    [[clang::nrvo]] return y; // OK
  }
  [[clang::nrvo]] return x; // expected-warning {{this return statement uses the 'nrvo' attribute, but NRVO did not happen}}
}
