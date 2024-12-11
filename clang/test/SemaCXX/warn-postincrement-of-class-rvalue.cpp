// RUN: %clang_cc1 -std=c++11 -fsyntax-only -Wassign-to-class-rvalue -verify %s
// RUN: %clang_cc1 -std=c++23 -fsyntax-only -Wassign-to-class-rvalue -verify %s

template<class T>
T f();

struct S {
  void operator--(int); // expected-note{{declared here}}
  S operator++(int); // expected-note{{declared here}}
};

struct SV {
  SV operator--(int) const;
  void operator++(int) const;
};

struct BitRef {
  void operator--(int) const;
  void operator++(int) const;
};

struct RQ {
  void operator--(int) &&;
  RQ& operator++(int) &&;
};

struct CQ {
  void operator--(int) const&;
  CQ operator++(int) const&;
};

void test() {
  f<S>()++;      // expected-warning{{possibly unintended postincrement of an rvalue of type 'S'}}
  f<SV>()++;     // OK, selected overload is const-qualified
  f<BitRef>()++; // OK
  f<RQ>()++;     // OK, selected overload is ref-qualified, which shows intent
  f<CQ>()++;     // OK, selected overload is ref-qualified, which shows intent

  f<S>()--;      // expected-warning{{possibly unintended postdecrement of an rvalue of type 'S'}}
  f<SV>()--;     // OK, selected overload is const-qualified
  f<BitRef>()--; // OK
  f<RQ>()--;     // OK, selected overload is ref-qualified, which shows intent
  f<CQ>()--;     // OK, selected overload is ref-qualified, which shows intent
}
