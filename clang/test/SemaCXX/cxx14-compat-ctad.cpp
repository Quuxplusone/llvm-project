// RUN: %clang_cc1 -fsyntax-only -std=c++17 -Wctad -verify %s
// RUN: %clang_cc1 -fsyntax-only -std=c++20 -Wctad -verify %s

template<typename T = int> struct X {};
X x; // expected-warning {{class template argument deduction is incompatible with C++ standards before C++17; for compatibility, use explicit type name 'X<>'}}

template<template<typename> class> struct Y {};
Y<X> yx; // ok, not class template argument deduction

template<typename T> void f(T t) {
  X x = t; // expected-warning {{class template argument deduction is incompatible with C++ standards before C++17}}
}

template<typename T> void g(T t) {
  typename T::X x = t; // expected-warning {{class template argument deduction is incompatible with C++ standards before C++17; for compatibility, use explicit type name 'typename A::X<A>' (aka 'A::X<A>')}}
}

struct A { template<typename T> struct X { X(T); }; };
void h(A a) { g(a); } // expected-note {{in instantiation of function template specialization 'g<A>'}}

template<class T> struct V { V(const T&) {} };

V(int) -> V<int*>;  // ok, deduction guide is not a use of class template argument deduction

void f2() { V v('a'); } // expected-warning {{class template argument deduction is incompatible with C++ standards before C++17; for compatibility, use explicit type name 'V<char>'}}
void g2() { V v(0); } // expected-warning {{class template argument deduction is incompatible with C++ standards before C++17; for compatibility, use explicit type name 'V<int *>'}}

void h2() {
    auto lam = [](){};
    V v(lam); // expected-warning {{class template argument deduction is incompatible with C++ standards before C++17}}
}
