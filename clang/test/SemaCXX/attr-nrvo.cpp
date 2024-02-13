// RUN: %clang_cc1 -verify -fsyntax-only -fms-extensions -fcxx-exceptions -fopenmp -triple x86_64-linux %s

[[clang::nrvo]] int g; // expected-error {{'nrvo' attribute cannot be applied to a declaration}}

void ReturnsVoid() {
  [[clang::nrvo]] return; // expected-error {{'nrvo' attribute requires that the returned expression}}
}

int ReturnsInt(int param) {
  int x = 42;
  [[clang::nrvo]] ReturnsVoid(); // // expected-error {{'nrvo' attribute only applies to return statements}}
  [[clang::nrvo]] return param; // expected-error {{'nrvo' attribute requires that the returned expression}}
  [[clang::nrvo]] return x+1; // expected-error {{'nrvo' attribute requires that the returned expression}}
}

struct S { S(); S(S&&); ~S(); };

S ReturnsS(S param) {
  S x;
  [[clang::nrvo]] return param; // expected-error {{'nrvo' attribute requires that the returned expression}}
  [[clang::nrvo]] return S(); // expected-error {{'nrvo' attribute requires that the returned expression}}
}

S ReturnsS2() {
  S x;
  [[clang::nrvo]] return x; // OK
}

struct NotS { NotS(const S&); NotS(NotS&&); ~NotS(); };
struct DerivedFromS : S { int x = 42; };

NotS ReturnsNotS() {
  S x;
  [[clang::nrvo]] return x; // expected-error {{'nrvo' attribute requires that the returned expression}}
}

S ReturnsByDerivedToBaseConversion() {
  DerivedFromS x;
  [[clang::nrvo]] return x; // expected-error {{'nrvo' attribute requires that the returned expression}}
}

struct CopyableS { CopyableS(); CopyableS(const CopyableS&); ~CopyableS(); };
struct PairOfS { CopyableS a; S b; };

CopyableS ReturnsStructuredBinding() {
  auto [x,y] = PairOfS();
  [[clang::nrvo]] return x; // expected-error {{'nrvo' attribute requires that the returned expression}}
}
