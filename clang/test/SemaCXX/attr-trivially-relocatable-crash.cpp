// RUN: %clang_cc1 -fsyntax-only -verify %s -std=c++11
// RUN: %clang_cc1 -fsyntax-only -verify %s -std=c++26

template<class C>
struct IsTR {
  static constexpr int value = __is_trivially_relocatable(C);
    // expected-error@-1 {{implicit instantiation of undefined template 'UndefinedTemplateCrash::Vector<int>'}}
    // expected-error@-2 {{incomplete type 'IncompleteTypeCrash::Incomplete' used in type trait expression}}
};

namespace UndefinedTemplateCrash { // fixed 2026-01-08
  template<class> struct Vector;
    // expected-note@-1 {{template is declared here}}

  template<class T>
  struct [[trivially_relocatable(IsTR<Vector<T>>::value)]] FlatSet {};
    // expected-note@-1 {{in instantiation of}}

  FlatSet<int> fs;
    // expected-note@-1 {{in instantiation of template class}}
}

namespace IncompleteTypeCrash { // fixed 2026-01-08
  struct Incomplete;
    // expected-note@-1 {{forward declaration}}
  template<Incomplete (*F)()>
  struct [[trivially_relocatable(IsTR<decltype(F())>::value)]] Other {};
    // expected-note@-1 {{in instantiation of}}

  Incomplete f();
  Other<f> o;
    // expected-note@-1 {{in instantiation of template class}}
}
