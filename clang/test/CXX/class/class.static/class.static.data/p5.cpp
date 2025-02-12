// RUN: %clang_cc1 -fsyntax-only -verify %s

int bar();

int foo1() {
    struct S {
        static constexpr int x = 1; // expected-warning {{static data member 'x' not allowed in local struct 'S'}}
        static int y;  // expected-warning {{static data member 'y' not allowed in local struct 'S'}}
        // expected-warning@-1 {{variable 'foo1()::S::y' has internal linkage but is not defined}}
        // expected-note@#foo1-assign {{used here}}
        static inline int z = bar();  // expected-warning {{static data member 'z' not allowed in local struct 'S'}}
    };
    static_assert(S::x == 1);
    S::y = 2; // #foo1-assign
    return S::z;
}

struct {
    static constexpr int x = 1; // expected-warning {{static data member 'x' not allowed in unnamed struct}}
    static int y;  // expected-warning {{static data member 'y' not allowed in unnamed struct}}
    // expected-warning@-1 {{variable '(anonymous struct)::y' has internal linkage but is not defined}}
    // expected-note@#foo2-assign {{used here}}
    static inline int z = bar(); // expected-warning {{static data member 'z' not allowed in unnamed struct}}
} s;

int foo2() {
    static_assert(s.x == 1);
    s.y = 2; // #foo2-assign
    return s.z;
}
