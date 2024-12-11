// RUN: %clang_cc1 -std=c++11 -fsyntax-only -Wpreincrement-of-class-rvalue -Wall -verify %s
// RUN: %clang_cc1 -std=c++23 -fsyntax-only -Wpreincrement-of-class-rvalue -Wall -verify %s

template<class T>
T f();

struct It {
  It& operator++();   // expected-note{{declared here}}
  It operator++(int); // expected-note{{declared here}}
  It& operator--();   // expected-note{{declared here}}
  It operator--(int); // expected-note{{declared here}}
};

struct AtomicRef {
  AtomicRef& operator++() const;
  int operator++(int) const;
  AtomicRef& operator--() const;
  int operator--(int) const;
};

struct BitRef {
  BitRef& operator++();
  const BitRef& operator++() const;
  BitRef operator++(int);
  BitRef operator++(int) const;
};

struct RQ {
  void operator++() &&;
  void operator++(int) &&;
};

struct CQ {
  void operator++() const&;
  void operator++(int) const&;
};

void test() {
  It it;
  ++it; // OK
  it++; // OK
  --it; // OK
  it--; // OK
  ++f<It>(); // expected-warning{{possibly unintended preincrement of an rvalue of type 'It'}}
  f<It>()++; // expected-warning{{possibly unintended postincrement of an rvalue of type 'It'}}
  --f<It>(); // expected-warning{{possibly unintended predecrement of an rvalue of type 'It'}}
  f<It>()--; // expected-warning{{possibly unintended postdecrement of an rvalue of type 'It'}}

  AtomicRef ar;
  ++ar; // OK
  ar++; // OK
  --ar; // OK
  ar--; // OK
  ++f<AtomicRef>(); // OK
  f<AtomicRef>()++; // OK
  --f<AtomicRef>(); // OK
  f<AtomicRef>()--; // OK

  BitRef br;
  ++br; // OK
  br++; // OK
  ++f<BitRef>(); // OK
  f<BitRef>()++; // OK

  ++f<RQ>(); // OK, selected overload is ref-qualified, which shows intent
  f<RQ>()++; // OK, selected overload is ref-qualified, which shows intent

  ++f<CQ>(); // OK, selected overload is ref-qualified, which shows intent
  f<CQ>()++; // OK, selected overload is ref-qualified, which shows intent
}
