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
  [[clang::nrvo]] return x; // expected-warning {{this return statement uses the 'clang::nrvo' attribute, but NRVO did not happen}}
}

struct TrivialDtor {
    explicit TrivialDtor();
    TrivialDtor(TrivialDtor&&);
};

TrivialDtor ReturnsWithNRVOOfTrivialDtor() {
    TrivialDtor x;
    [[clang::nrvo]] return x; // OK
}

struct CompletelyTrivial {
  int i_;
};

CompletelyTrivial ReturnsWithCompletelyTrivialInRegister() {
  CompletelyTrivial x;
  [[clang::nrvo]] return x; // OK
}

template<class T>
T TemplateReturnsWithoutNRVO(bool cond) {
  T x;
  if (cond) {
    T y;
    [[clang::nrvo]] return y; // OK
  }
  [[clang::nrvo]] return x; // expected-warning 3 {{this return statement uses the 'clang::nrvo' attribute, but NRVO did not happen}}
}

template S TemplateReturnsWithoutNRVO<S>(bool);
template TrivialDtor TemplateReturnsWithoutNRVO<TrivialDtor>(bool);
template CompletelyTrivial TemplateReturnsWithoutNRVO<CompletelyTrivial>(bool);

struct NotS { NotS(); operator S() const; };
struct DerivedFromS : S { int x = 42; };

template<class T>
S ReturnsMaybeNotS() {
  T x;
  [[clang::nrvo]] return x; // expected-warning 2 {{this return statement uses the 'clang::nrvo' attribute, but NRVO did not happen}}
}

template S ReturnsMaybeNotS<S>();
template S ReturnsMaybeNotS<NotS>();
template S ReturnsMaybeNotS<DerivedFromS>();
