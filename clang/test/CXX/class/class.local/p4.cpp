// RUN: %clang_cc1 -fsyntax-only -verify %s

void f() {
  struct X {
    static int a; // expected-warning {{static data member 'a' not allowed in local struct 'X'}}
    struct S {
      static int b; // expected-warning {{static data member 'b' not allowed in local struct 'S'}}
    };
    int b;

    static void f() { }
  };
  union A {
    static int a; // expected-warning {{static data member 'a' not allowed in local union 'A'}}
    struct S {
      static int b; // expected-warning {{static data member 'b' not allowed in local struct 'S'}}
    } s;
    int c;

    static void f() { }
  };
  union {
    static int a; // expected-warning {{static data member 'a' not allowed in unnamed union}}
    struct T {
      static int b; // expected-warning {{static data member 'b' not allowed in unnamed union}}
    } t;
  } u;
}

struct {
  static int a; // expected-warning {{static data member 'a' not allowed in unnamed struct}}
  union T {
    static int b; // expected-warning {{static data member 'b' not allowed in unnamed struct}}
  } t;
} s;

union {
  static int a; // expected-warning {{static data member 'a' not allowed in unnamed union}}
  struct T {
    static int b; // expected-warning {{static data member 'b' not allowed in unnamed union}}
  } t;
} u;

struct ContainsAnonymousUnionMember {
  union {
    struct {
      static int f() {
        struct Local {
          static inline int s = 1; // expected-warning {{static data member 's' not allowed in local struct 'Local'}}
        };
        return Local::s;
      }
    } s;
  };
};

int test() {
  return ContainsAnonymousUnionMember().s.f();
}
