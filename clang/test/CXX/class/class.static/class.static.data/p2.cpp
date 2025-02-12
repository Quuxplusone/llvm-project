// RUN: %clang_cc1 -std=c++1z -verify %s

struct X {
  static struct A a;
  static inline struct B b; // expected-error {{incomplete type}} expected-note {{forward decl}}
  static inline struct C c = {}; // expected-error {{incomplete type}} expected-note {{forward decl}}
};

union UnionType {
    int i;
    static inline int s; // OK
};

struct AnonymousUnionMember {
    union {
        int i;
        static inline int s = 0; // expected-warning {{static data member 's' not allowed in unnamed union}}
        // expected-error@-1 {{static members cannot be declared in an anonymous union}}
    };
};

struct UnnamedUnionMember {
    union {
        int i;
        static inline int s = 0; // expected-warning {{static data member 's' not allowed in unnamed union}}
    } u;
};

int foo3() {
    union UnionType {
        int i;
        static inline int s = 0; // expected-warning {{static data member 's' not allowed in local union 'UnionType'}}
    };
    union {
        int i;
        static inline int s = 0; // expected-warning {{static data member 's' not allowed in unnamed union}}
    } u;
    return u.s + UnionType().s;
}
