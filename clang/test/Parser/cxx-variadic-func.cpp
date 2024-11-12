// RUN: %clang_cc1 -fsyntax-only -verify %s

void f(...) {
  // FIXME: There's no disambiguation here; this is unambiguous.
  int g(int(...)); // expected-warning {{disambiguated}} expected-note {{paren}}
}

void h(int n..., int m); // expected-error {{expected ')'}} expected-note {{to match}}
  // expected-warning@-1 {{variadic ellipsis without a leading comma is deprecated in C++26}}
