// RUN: %clang_cc1 -fsyntax-only -verify -std=c++03 %s
// RUN: %clang_cc1 -fsyntax-only -verify -std=c++11 %s
// RUN: %clang_cc1 -fsyntax-only -verify -std=c++14 %s
// RUN: %clang_cc1 -fsyntax-only -verify -std=c++17 %s

struct A {};
struct B : A { B(); explicit B(A); };
struct C : private A {};
typedef float& FloatRef;
typedef int& IntRef;
typedef A& ARef;
typedef B& BRef;
typedef C& CRef;
typedef A const& AConstRef;
typedef B const& BConstRef;

A a;
B b;
C c;
int i;
float f;
const int ci = 42;
const B cb = B();

void test() {
    short x1 = short(f); // OK
    short x2 = short(i); // OK
    float x3 = float(f); // OK
    float x4 = float(i); // OK
    int x5 = int(f);     // OK
    int x6 = int(i);     // OK
    bool x7 = bool(f);   // OK
    bool x8 = bool(i);   // OK

    A x9 = A(b);         // OK, slicing cast to base
    ARef x10 = ARef(b);  // OK, cast to base
    B x11 = B(a);        // OK, constructor
    BRef x12 = BRef(a);  // OK, static_cast can also do this downcast
    CRef x14 = CRef(a);  // OK, static_cast can also do this downcast
    ARef x16 = ARef(c);  // ERR, cast to private base
    ARef x18 = ARef(i);  // ERR, reinterpret_cast
                         // expected-warning@-1{{functional-style cast is not equivalent to static_cast}}

    int x19 = IntRef(f);    // ERR, reinterpret_cast
                         // expected-warning@-1{{functional-style cast is not equivalent to static_cast}}
    int x20 = FloatRef(i);  // ERR, reinterpret_cast
                         // expected-warning@-1{{functional-style cast is not equivalent to static_cast}}
    int x21 = IntRef(i);    // OK
    int x22 = FloatRef(f);  // OK
    int x23 = float(true);  // OK
    int x24 = bool(3.14);   // OK
    int x25 = int(ci);      // OK
    int x26 = IntRef(ci);   // ERR, const_cast
    int x27 = FloatRef(ci); // ERR, const_cast followed by reinterpret_cast
                         // expected-warning@-1{{functional-style cast is not equivalent to static_cast}}
    A x28 = ARef(cb);       // ERR, const_cast followed by cast to base
}
