// RUN: %clang_cc1 -std=c++11 -fsyntax-only -Wassign-to-class-rvalue -Wcxx23-compat-assign-to-class-rvalue -verify %s
// RUN: %clang_cc1 -std=c++23 -fsyntax-only -Wassign-to-class-rvalue -verify %s

template<class T>
T f();

struct S {
  S& operator=(const S&); // expected-note 2 {{declared here}}
};

struct SV {
  const SV& operator=(const SV&) const;
};

struct Triv {}; // expected-note 2 {{declared here}}

struct BitRef {
  BitRef& operator=(const BitRef&);
  const BitRef& operator=(const BitRef&) const;
};

struct RQ {
  void operator=(const RQ&) &&;
};

struct CQ {
  void operator=(const CQ&) const&;
};

void test() {
  S s;
  s = f<S>(); // OK
  f<S>() = s; // expected-warning{{possibly unintended assignment to an rvalue of type 'S'}}

  SV sv;
  sv = f<SV>(); // OK
  f<SV>() = sv; // OK, selected overload is const-qualified

  Triv t;
  t = f<Triv>(); // OK
  f<Triv>() = t; // expected-warning{{possibly unintended assignment to an rvalue of type 'Triv'}}

  BitRef b;
  b = f<BitRef>(); // OK
  f<BitRef>() = b; // OK

  RQ rq;
  f<RQ>() = rq; // OK, selected overload is ref-qualified, which shows intent

  CQ cq;
  cq = f<CQ>(); // OK
  f<CQ>() = cq; // OK, selected overload is ref-qualified, which shows intent
}

template<class T>
void test_template() {
  T t;
  t = f<T>(); // OK
  f<T>() = t;
    // expected-warning@-1{{possibly unintended assignment to an rvalue of type 'S'}}
    // expected-warning@-2{{possibly unintended assignment to an rvalue of type 'Triv'}}
}

template void test_template<S>();      // expected-note{{requested here}}
template void test_template<SV>();
template void test_template<Triv>();   // expected-note{{requested here}}
template void test_template<BitRef>();
template void test_template<CQ>();


struct Compound {
  Compound& operator+=(const Compound&); // expected-note{{declared here}}
};
struct CompoundRef {
  CompoundRef& operator+=(const CompoundRef&);
  const CompoundRef& operator+=(const CompoundRef&) const;
};

void test_compound() {
  Compound c;
  c += f<Compound>(); // OK
  f<Compound>() += c; // expected-warning{{possibly unintended assignment to an rvalue of type 'Compound'}}

  CompoundRef r;
  r += f<CompoundRef>(); // OK
  f<CompoundRef>() += r; // OK
}
