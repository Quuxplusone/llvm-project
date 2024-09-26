// RUN: %clang_cc1 %s -std=c++20 -fcxx-exceptions -fexceptions -fsyntax-only -Wignored-qualifiers -verify
// RUN: %clang_cc1 %s -std=c++20 -fcxx-exceptions -fexceptions -fsyntax-only -Wignored-qualifiers -Wqual-class-return-type -verify

struct Tr { explicit Tr(int); };
struct NTr { explicit NTr(int); NTr(const NTr&); ~NTr(); };

const int fint(); // expected-warning {{'const' type qualifier on return type has no effect}}
const Tr ftr();   // expected-warning {{'const' type qualifier on return type is a bad idea}}
const NTr fntr(); // expected-warning {{'const' type qualifier on return type is a bad idea}}

template<class T> T f() { return T(1); }
template<class T> const T fconst(int& x) { return T(x); } // expected-warning {{'const' type qualifier on return type is a bad idea}}

// TODO: These should be OK, since we can't control the return type here.
template const char f<const char>(); // expected-warning {{'const' type qualifier on return type has no effect}}
template const Tr f<const Tr>();     // expected-warning {{'const' type qualifier on return type is a bad idea}}
template const NTr f<const NTr>();   // expected-warning {{'const' type qualifier on return type is a bad idea}}
template const char fconst<char>(int&);  // expected-warning {{'const' type qualifier on return type has no effect}}
template const Tr fconst<Tr>(int&);      // expected-warning {{'const' type qualifier on return type is a bad idea}}
template const NTr fconst<NTr>(int&);    // expected-warning {{'const' type qualifier on return type is a bad idea}}

// TODO: These should be OK, since we can't control the return type here.
template<class T> const T g();                     // expected-warning {{'const' type qualifier on return type is a bad idea}}
template<> const int g<int>() { return 42; }       // expected-warning {{'const' type qualifier on return type has no effect}}
template<> const Tr g<Tr>() { return Tr(42); }     // expected-warning {{'const' type qualifier on return type is a bad idea}}
template<> const NTr g<NTr>() { return NTr(42); }  // expected-warning {{'const' type qualifier on return type is a bad idea}}

void test(int i) {
  f<const int>(); // OK, it's a template
  f<const Tr>();  // OK, it's a template
  f<const NTr>(); // OK, it's a template
  fconst<int>(i);  // OK, it's a template
  fconst<Tr>(i);   // OK, it's a template
  fconst<NTr>(i);  // OK, it's a template
  fconst<int&>(i); // OK, it's a template
}

const int declint();      // expected-warning {{'const' type qualifier on return type has no effect}}
const Tr decltr();        // expected-warning {{'const' type qualifier on return type is a bad idea}}
const NTr declntr();      // expected-warning {{'const' type qualifier on return type is a bad idea}}
const auto declunknown(); // TODO: Ideally, we'd give a warning here too.

const auto fint2() { return 1; }      // expected-warning {{'const' type qualifier on return type has no effect}}
const auto ftr2() { return Tr(1); }   // expected-warning {{'const' type qualifier on return type is a bad idea}}
const auto fntr2() { return NTr(1); } // expected-warning {{'const' type qualifier on return type is a bad idea}}

auto lint2 = []() -> const auto { return 1; };      // expected-warning {{'const' type qualifier on return type has no effect}}
auto ltr2 = []() -> const auto { return Tr(1); };   // expected-warning {{'const' type qualifier on return type is a bad idea}}
auto lntr2 = []() -> const auto { return NTr(1); }; // expected-warning {{'const' type qualifier on return type is a bad idea}}

template<class T>
struct Dependent {
  static const auto f() { return T(1); }
    // expected-warning@-1 1 {{'const' type qualifier on return type has no effect}}
    // expected-warning@-2 2 {{'const' type qualifier on return type is a bad idea}}
  static const T fconst(int& x) { return T(x); } // expected-warning {{'const' type qualifier on return type is a bad idea}}
};

void test_dependent(int i) {
  fint2();
  ftr2();
  fntr2();
  Dependent<const int>::f(); // expected-note {{in instantiation of member function 'Dependent<const int>::f' requested here}}
  Dependent<const Tr>::f();  // expected-note {{in instantiation of member function 'Dependent<const Tr>::f' requested here}}
  Dependent<const NTr>::f(); // expected-note {{in instantiation of member function 'Dependent<const NTr>::f' requested here}}
  Dependent<int>::fconst(i);
  Dependent<Tr>::fconst(i);
  Dependent<NTr>::fconst(i);
  Dependent<int&>::fconst(i);
}
