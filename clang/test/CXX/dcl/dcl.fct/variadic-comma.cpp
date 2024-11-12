// RUN: %clang_cc1 -fsyntax-only -verify %s
// RUN: %clang_cc1 -fsyntax-only -verify -std=c++11 %s
// RUN: %clang_cc1 -fsyntax-only -verify -std=c++26 %s

void f1(...);
void f2(int, ...);
void f3(int x, ...);
void f4(int ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
void f5(int x ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
template<class T> void g1(...);
template<class T> void g2(T, ...);
template<class T> void g3(T t, ...);
template<class T> void g4(T ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
template<class T> void g5(T t ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
template<class T> void g6(int ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
template<class T> void g7(int x ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
template<class... Ts> void h1(...);
template<class... Ts> void h2(Ts...);
template<class... Ts> void h3(Ts... ts);
template<class... Ts> void h4(Ts..., ...);
template<class... Ts> void h5(Ts... ts, ...);
template<class... Ts> void h6(Ts... ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
  // expected-warning@-1 {{'...' in this location creates a C-style varargs function}}
  // expected-note@-2 {{preceding '...' declares a function parameter pack}}
  // expected-note@-3 {{insert ',' before '...' to silence this warning}}
template<class... Ts> void h7(Ts... ts ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
  // expected-warning@-1 {{'...' in this location creates a C-style varargs function}}
  // expected-note@-2 {{preceding '...' declares a function parameter pack}}
  // expected-note@-3 {{insert ',' before '...' to silence this warning}}
template<class... Ts> void h8(Ts ts ...); // expected-error{{'...' must immediately precede declared identifier}}
template<class... Ts> void h9(Ts ts, ...); // expected-error{{declaration type contains unexpanded parameter pack 'Ts'}}

using T1 = void(*)(...);
using T2 = void(*)(int, ...);
using T3 = void(*)(int x, ...);
using T4 = void(*)(int ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
using T5 = void(*)(int x ...); // expected-warning{{variadic ellipsis without a leading comma is deprecated in C++26}}
