// RUN: %clang_cc1 -fsyntax-only -verify %s -std=c++11
// expected-diagnostics

static_assert(__has_extension(trivially_relocatable), "");

// It shall appear at most once in each attribute-list
// and no attribute-argument-clause shall be present.

struct [[clang::trivially_relocatable, clang::trivially_relocatable]] B1 {}; // should really be an error

struct [[clang::trivially_relocatable]] [[clang::trivially_relocatable]] B2 {}; // should really be an error

struct [[clang::trivially_relocatable(42)]] B3 {};

struct [[trivially_relocatable, trivially_relocatable]] B4 {};
struct [[trivially_relocatable]] [[trivially_relocatable]] B5 {}; // should really be an error
struct [[trivially_relocatable(42)]] B6 {};
struct [[trivially_relocatable, clang::trivially_relocatable]] B7 {};
struct [[trivially_relocatable]] [[clang::trivially_relocatable]] B8 {}; // should really be an error


//   The first declaration of a type shall specify the
//   trivially_relocatable attribute if any declaration of that
//   type specifies the trivially_relocatable attribute.

struct [[clang::trivially_relocatable]] A1 {};  // ok
struct [[clang::trivially_relocatable]] A1;

struct [[clang::trivially_relocatable]] A2;  // ok
struct [[clang::trivially_relocatable]] A2 {};

struct [[clang::trivially_relocatable]] A3 {};  // ok
struct A3;

struct [[clang::trivially_relocatable]] A4;  // ok
struct A4 {};

struct A5 {};
struct [[clang::trivially_relocatable]] A5;
// expected-warning@-1{{attribute declaration must precede definition}}
// expected-note@-3{{previous definition is here}}

struct A6;
struct [[clang::trivially_relocatable]] A6 {};


// If a type T is declared with the trivially_relocatable attribute, and T is
// either not move-constructible or not destructible, the program is ill-formed.

struct NonDestructible {
    NonDestructible(const NonDestructible&) = default;
    NonDestructible(NonDestructible&&) = default;
    ~NonDestructible() = delete;
};
struct NonCopyConstructible {
    NonCopyConstructible(const NonCopyConstructible&) = delete;
};
struct NonMoveConstructible {
    NonMoveConstructible(const NonMoveConstructible&) = default;
    NonMoveConstructible(NonMoveConstructible&&) = delete;
    // expected-note@-1{{explicitly marked deleted here}}
};
static_assert(!__is_trivially_relocatable(NonDestructible), "");
static_assert(!__is_trivially_relocatable(NonCopyConstructible), "");
static_assert(!__is_constructible(NonCopyConstructible, NonCopyConstructible&&), "");
static_assert(!__is_trivially_relocatable(NonMoveConstructible), "");
static_assert(!__is_constructible(NonMoveConstructible, NonMoveConstructible&&), "");

struct [[clang::trivially_relocatable]] D1 { ~D1() = delete; };
// expected-error@-1{{cannot be applied to struct 'D1' because it is not destructible}}

struct [[clang::trivially_relocatable]] D2 : private NonDestructible { };
// expected-error@-1{{cannot be applied to struct 'D2' because it is not destructible}}

struct [[clang::trivially_relocatable]] D3 { D3(const D3&) = delete; };
// expected-error@-1{{cannot be applied to struct 'D3' because it is not move-constructible}}

struct [[clang::trivially_relocatable]] D4 { D4(const D4&) = default; D4(D4&&) = delete; };
// expected-error@-1{{cannot be applied to struct 'D4' because it is not move-constructible}}

struct [[clang::trivially_relocatable]] D5 : private NonCopyConstructible { };
// expected-error@-1{{cannot be applied to struct 'D5' because it is not move-constructible}}
static_assert(!__is_constructible(D5, D5&&), "");

struct [[clang::trivially_relocatable]] D6 : private NonMoveConstructible { D6(D6&&) = default; };
// expected-warning@-1{{explicitly defaulted move constructor is implicitly deleted}}
// expected-note@-2{{implicitly deleted because}}
// expected-note@-3{{replace 'default' with 'delete'}}
// expected-error@-4{{cannot be applied to struct 'D6' because it is not move-constructible}}

template<class T>
struct [[clang::trivially_relocatable]] DT1 : private T { };  // ok

struct D7 {
    DT1<NonDestructible> m;
};

class [[clang::trivially_relocatable]] D8 {
    DT1<NonDestructible> m;
};
// expected-error@-3{{cannot be applied to class 'D8' because it is not destructible}}


// Now test specific types for trivial relocatability.

static_assert(__is_trivially_relocatable(char), "");
static_assert(__is_trivially_relocatable(int), "");
static_assert(__is_trivially_relocatable(int*), "");
static_assert(__is_trivially_relocatable(int(*)()), "");
static_assert(!__is_trivially_relocatable(int&), "");
static_assert(!__is_trivially_relocatable(int(&)()), "");
static_assert(!__is_trivially_relocatable(int()), "");
static_assert(__is_trivially_relocatable(float), "");
static_assert(__is_trivially_relocatable(double), "");
static_assert(!__is_trivially_relocatable(void), "");
static_assert(__is_trivially_relocatable(char[1]), "");
static_assert(__is_trivially_relocatable(char[]), "");
static_assert(__is_trivially_relocatable(char[2][3]), "");
static_assert(__is_trivially_relocatable(char[][3]), "");
static_assert(__is_trivially_relocatable(const char[2][3]), "");
static_assert(__is_trivially_relocatable(const char[][3]), "");
static_assert(__is_trivially_relocatable(volatile char[2][3]), "");
static_assert(__is_trivially_relocatable(volatile char[][3]), "");

static_assert(__is_trivially_relocatable(const int), "");
static_assert(__is_trivially_relocatable(volatile int), "");
static_assert(!__is_trivially_relocatable(const int&), "");
static_assert(!__is_trivially_relocatable(volatile int&), "");

struct C1 { int x; }; static_assert(__is_trivially_relocatable(C1), "");
struct C2 { const int x; }; static_assert(__is_trivially_relocatable(C2), "");
struct C3 { volatile int x; }; static_assert(__is_trivially_relocatable(C3), "volatile member OK since P1144R3");
struct C4 { int *x; }; static_assert(__is_trivially_relocatable(C4), "");
struct C5 { const int *x; }; static_assert(__is_trivially_relocatable(C5), "");
struct C6 { volatile int *x; }; static_assert(__is_trivially_relocatable(C6), "");
struct C7 { int& x; }; static_assert(__is_trivially_relocatable(C7), "");
struct C8 { const int& x; }; static_assert(__is_trivially_relocatable(C8), "");
struct C9 { volatile int& x; }; static_assert(__is_trivially_relocatable(C9), "");

enum E { x = 1, y = 2 };
static_assert(__is_trivially_relocatable(E), "");
static_assert(__is_trivially_relocatable(E[1]), "");

struct T1 {};
static_assert(__is_trivially_relocatable(T1), "");

struct T2 { int x; E y; int *z; };
static_assert(__is_trivially_relocatable(T2), "");

struct T3 { int x; T3(T3&&) = default; };
static_assert(__is_trivially_relocatable(T3), "");

struct T4 { int x; ~T4() = default; };
static_assert(__is_trivially_relocatable(T4), "trivially copy-constructible, and no move constructor");

struct T4a { T4 a; };
static_assert(__is_trivially_relocatable(T4a), "trivially copy-constructible, and no move constructor");


struct VD {
    VD(const VD&) = default;
    VD(VD&&) = default;
    virtual ~VD() = default;
};
void relocate_example(VD&& src) {
    VD dst(static_cast<VD&&>(src));  // this DEFINITELY calls the trivial move-constructor
    src.~VD();  // this MAY virtually dispatch to a non-trivial destructor
}
static_assert(!__is_trivially_relocatable(VD), "");
static_assert(!__is_trivially_relocatable(VD[1]), "");
static_assert(!__is_trivially_relocatable(VD[]), "");
static_assert(!__is_trivially_relocatable(VD[2][3]), "");
static_assert(!__is_trivially_relocatable(VD[][3]), "");
static_assert(!__is_trivially_relocatable(const VD[2][3]), "");
static_assert(!__is_trivially_relocatable(const VD[][3]), "");
static_assert(!__is_trivially_relocatable(volatile VD[2][3]), "");
static_assert(!__is_trivially_relocatable(volatile VD[][3]), "");

struct VD2 final {
    VD2(const VD2&) = default;
    VD2(VD2&&) = default;
    virtual ~VD2() = default;
};
void relocate_example(VD2&& src) {
    VD2 dst(static_cast<VD2&&>(src));  // this DEFINITELY calls the trivial move-constructor
    src.~VD2();  // because "final", this CANNOT virtually dispatch to a non-trivial destructor
}
static_assert(!__is_trivially_relocatable(VD2), "VD2 still has a vptr, which P1144R5 doesn't allow");


struct VD3 {
    VD3(const VD3&) = default;
    VD3(VD3&&) = default;
    virtual ~VD3() final = default;
    // expected-warning@-1{{class with destructor marked 'final'}}
    // expected-note@-5{{mark 'VD3' as 'final' to silence this warning}}
};
void relocate_example(VD3&& src) {
    VD3 dst(static_cast<VD3&&>(src));  // this DEFINITELY calls the trivial move-constructor
    src.~VD3();  // because "final", this CANNOT virtually dispatch to a non-trivial destructor
}
static_assert(!__is_trivially_relocatable(VD3), "VD3 still has a vptr, which P1144R5 doesn't allow");


struct VB : public virtual T1 {
    VB(const VB&) = default;
    VB(VB&&) = default;
    ~VB() = default;
};
void relocate_example(VB&& src) {
    VB dst(static_cast<VB&&>(src));  // this MAY copy the virtual bases of "src" in a way not tantamount to memcpy
    src.~VB();  // this calls the trivial destructor
}
static_assert(__is_trivially_destructible(VB), "");
static_assert(!__is_trivially_constructible(VB, VB&&), "");
static_assert(!__is_trivially_relocatable(VB), "");

struct VB2 final : public virtual T1 {
    VB2(const VB2&) = default;
    VB2(VB2&&) = default;
    ~VB2() = default;
};
void relocate_example(VB2&& src) {
    VB2 dst(static_cast<VB2&&>(src));  // this MAY STILL copy the VBPTR of "src" in a way not tantamount to memcpy
    src.~VB2();  // this calls the trivial destructor
}
static_assert(__is_trivially_destructible(VB2), "");
static_assert(!__is_trivially_constructible(VB2, VB2&&), "");
static_assert(!__is_trivially_relocatable(VB2), "");


struct CCNMC {
    CCNMC(const CCNMC&) = default;
    // no move constructor at all
    ~CCNMC() = default;
};
void relocate_example(CCNMC&& src) {
    CCNMC dst(static_cast<CCNMC&&>(src));  // this calls the trivial copy-constructor
    src.~CCNMC();  // this calls the trivial destructor
}
static_assert(__is_constructible(CCNMC, CCNMC&&), "");
static_assert(__is_trivially_relocatable(CCNMC), "");


struct CCDMC {
    CCDMC(const CCDMC&) = default;
    CCDMC(CCDMC&&) = delete;
    ~CCDMC() = default;
};
void relocate_example(CCDMC&& src) {
    // CCDMC dst(static_cast<CCDMC&&>(src));  // this is not permitted
    src.~CCDMC();  // this calls the trivial destructor
}
static_assert(!__is_constructible(CCDMC, CCDMC&&), "");
static_assert(!__is_trivially_relocatable(CCDMC), "");

struct DD { ~DD() = delete; };
static_assert(__is_trivially_copyable(DD), "");
static_assert(!__is_trivially_destructible(DD), "");
static_assert(!__is_trivially_relocatable(DD), "");


struct T5 { int x; T5(T5&&) {} };
static_assert(!__is_trivially_relocatable(T5), "");

struct T6 { int x; ~T6() {} };
static_assert(!__is_trivially_relocatable(T6), "");

struct T7 { int x; T7(const T7&) {} };
static_assert(!__is_trivially_relocatable(T7), "T7 has no implicitly declared move constructor");

struct T8 { virtual void f() {} int x; };
static_assert(!__is_trivially_relocatable(T8), "T8 has a vptr");

struct T8a { virtual void g() = 0; int x; };
static_assert(!__is_trivially_relocatable(T8a), "T8a has a vptr");

struct [[clang::trivially_relocatable]] T8b { virtual void g(); int x; };
static_assert(__is_trivially_relocatable(T8b), "T8b has a vptr, but it has the attribute");

struct T8c : private T8b { };
static_assert(!__is_trivially_relocatable(T8c), "T8c has a vptr and doesn't have the attribute");

struct [[clang::trivially_relocatable]] T9 { int x; T9(T9&&) {} };
static_assert(__is_trivially_relocatable(T9), "T9 isn't naturally, but it has the attribute");

struct [[clang::trivially_relocatable]] T10 { int x; ~T10() {} };
static_assert(__is_trivially_relocatable(T10), "T10 isn't naturally, but it has the attribute");

struct T11 {
    T11();
    T1 a;
    T2 b;
    T3 c;
    T4 d;
    T8b e;
    T9 f;
    T10 g;
};
static_assert(__is_trivially_relocatable(T11), "all fields have trivially relocatable types");

struct T12 {
    T1 a;
    T2 b;
    T3 c;
    T5 d;  // not trivially relocatable
    T8b e;
    T9 f;
    T10 g;
};
static_assert(!__is_trivially_relocatable(T12), "not all fields have trivially relocatable types");

struct T13 : T1, T2, T3, T4 {};
static_assert(__is_trivially_relocatable(T13), "all bases have trivially relocatable types");

struct T14 : T1, T6, T3, T4 {};
static_assert(!__is_trivially_relocatable(T14), "not all bases have trivially relocatable types");

template<class... Ts>
struct T15 : Ts... {};

static_assert(__is_trivially_relocatable(T15<T1,T2,T3>), "all bases have trivially relocatable types");
static_assert(!__is_trivially_relocatable(T15<T1,T6,T3>), "not all bases have trivially relocatable types");

template<class... Ts>
struct [[clang::trivially_relocatable]] T16 : Ts... {};

static_assert(__is_trivially_relocatable(T16<T1,T2,T3>), "all bases have trivially relocatable types");
static_assert(__is_trivially_relocatable(T16<T1,T6,T3>), "not naturally, but it has the attribute");

struct T17 : T15<T10> {};  // T10 is trivially relocatable
static_assert(__is_trivially_relocatable(T17), "");
static_assert(__is_trivially_relocatable(T15<T17>), "");
static_assert(__is_trivially_relocatable(T16<T17>), "");

struct T17a : T15<T9> {};  // T9 is trivially relocatable
static_assert(__is_trivially_relocatable(T17a), "");
static_assert(__is_trivially_relocatable(T15<T17a>), "");
static_assert(__is_trivially_relocatable(T16<T17a>), "");

static_assert(__is_trivially_relocatable(T15<T17, T17a>), "");
static_assert(__is_trivially_relocatable(T16<T17, T17a>), "");

struct T18 : T15<T12> {};  // T12 is not trivially relocatable
static_assert(!__is_trivially_relocatable(T18), "");
static_assert(!__is_trivially_relocatable(T15<T18>), "");
static_assert(__is_trivially_relocatable(T16<T18>), "not naturally, but it has the attribute");


// This pattern WAS used heavily by libc++, but now it can use [[trivially_relocatable(bool)]] instead.
struct T19 {
    struct [[clang::trivially_relocatable]] Base {
        Base(Base&&);
        ~Base();
    };
    Base m;
    T19(const T19&);
    T19(T19&&) = default;
};

static_assert(!__is_trivially_constructible(T19, const T19&), "user-provided copy constructor");
static_assert(!__is_trivially_constructible(T19, T19&&), "defaulted non-trivial move constructor");
static_assert(!__is_trivially_destructible(T19), "defaulted non-trivial destructor");
static_assert(!__is_trivially_relocatable(T19), "user-provided copy constructor");


struct T20 {
    struct [[clang::trivially_relocatable]] SharedPtr {
        SharedPtr();
        SharedPtr(const SharedPtr&);
        SharedPtr(SharedPtr&&);
        ~SharedPtr();
    };
    SharedPtr m;
    T20(const T20&) = default;
    ~T20() = default;
    // no move constructor
};
void relocate_example(T20&& src) {
    T20 dst(static_cast<T20&&>(src));  // this calls the defaulted copy constructor and makes a COPY of the SharedPtr
    src.~T20();  // this calls the destructor and deletes the original copy
}
static_assert(__is_trivially_relocatable(T20::SharedPtr), "because it's annotated");
static_assert(!__is_trivially_constructible(T20, const T20&), "defaulted, non-trivial copy constructor");
static_assert(__is_constructible(T20, T20&&), "uses the copy constructor");
static_assert(!__is_trivially_constructible(T20, T20&&), "uses the copy constructor");
static_assert(!__is_trivially_destructible(T20), "defaulted non-trivial destructor");
static_assert(__is_trivially_relocatable(T20), "I'm not sure but I think copy-and-destroy should always be assumed tantamount to move-and-destroy");


struct T21 {
    struct [[clang::trivially_relocatable]] SharedPtr {
        SharedPtr();
        SharedPtr(const SharedPtr&);
        SharedPtr(SharedPtr&&);
        ~SharedPtr();
    };
    SharedPtr m;
    T21(const T21&);  // user-provided
    ~T21() = default;
    // no move constructor
};
void relocate_example(T21&& src) {
    T21 dst(static_cast<T21&&>(src));  // this calls the user-provided copy constructor
    src.~T21();  // this calls the defaulted destructor
}
static_assert(__is_trivially_relocatable(T21::SharedPtr), "because it's annotated");
static_assert(!__is_trivially_constructible(T21, const T21&), "non-defaulted, non-trivial copy constructor");
static_assert(__is_constructible(T21, T21&&), "uses the copy constructor");
static_assert(!__is_trivially_constructible(T21, T21&&), "uses the copy constructor");
static_assert(!__is_trivially_destructible(T21), "defaulted non-trivial destructor");
static_assert(!__is_trivially_relocatable(T21), "Relocating T21 calls T21's user-provided copy constructor, which we don't know what it does");


struct T22 {
    struct [[clang::trivially_relocatable]] MoveOnly { MoveOnly(MoveOnly&&); };
    struct CopyOnly { ~CopyOnly() = default; };
    MoveOnly m1;
    CopyOnly m2;
};
void relocate_example(T22&& src) {
    T22 dst(static_cast<T22&&>(src));  // this moves m1 (user-provided) and copies m2 (trivial, defaulted)
    src.~T22();  // this destroys m1 (trivial, defaulted) and m2 (trivial, defaulted)
}
static_assert(!__is_constructible(T22::MoveOnly, const T22::MoveOnly&), "");
static_assert(__is_constructible(T22::MoveOnly, T22::MoveOnly&&), "");
static_assert(!__is_trivially_constructible(T22::MoveOnly, T22::MoveOnly&&), "");
static_assert(__is_trivially_relocatable(T22::MoveOnly), "because it's annotated");
static_assert(__is_constructible(T22::CopyOnly, const T22::CopyOnly&), "");
static_assert(__is_constructible(T22::CopyOnly, T22::CopyOnly&&), "");
static_assert(__is_trivially_constructible(T22::CopyOnly, const T22::CopyOnly&), "");
static_assert(__is_trivially_constructible(T22::CopyOnly, T22::CopyOnly&&), "");
static_assert(__is_trivially_relocatable(T22::CopyOnly), "because its copy constructor is defaulted and its move constructor doesn't exist");
static_assert(!__is_constructible(T22, const T22&), "m1 is not copyable");
static_assert(__is_constructible(T22, T22&&), "");
static_assert(!__is_trivially_constructible(T22, T22&&), "m2 is not trivially moveable");
static_assert(__is_trivially_destructible(T22), "both members are trivially destructible");
static_assert(__is_trivially_relocatable(T22), "because its members are trivially relocatable");


struct T23 {
    struct Evil {
        Evil(Evil&);
        Evil(Evil&&) = default;
        ~Evil() = default;
    };
    mutable Evil m;
};
void relocate_example(T23&& src) {
    T23 dst(static_cast<T23&&>(src));  // this moves m (trivial, defaulted)
    src.~T23();  // this destroys m (trivial, defaulted)
}
static_assert(__is_trivially_constructible(T23::Evil, T23::Evil&&), "");
static_assert(__is_trivially_destructible(T23::Evil), "");
static_assert(!__is_trivially_relocatable(T23::Evil), "because it has a user-provided copy constructor");
static_assert(__is_constructible(T23, T23&), "");
static_assert(!__is_constructible(T23, const T23&), "");
static_assert(__is_trivially_constructible(T23, T23&&), "");
static_assert(__is_trivially_destructible(T23), "");
static_assert(!__is_trivially_relocatable(T23), "because it has a non-trivially relocatable member");

struct T23a {
    struct Evil {
        Evil(Evil&);
        Evil(const Evil&) = default;
        ~Evil() = default;
    };
    mutable Evil m;
};
void relocate_example(T23a&& src) {
    T23a dst(static_cast<T23a&&>(src));  // this copies m using the non-defaulted copy constructor
    src.~T23a();  // this destroys m (trivial, defaulted)
}
static_assert(__is_trivially_constructible(T23a::Evil, T23a::Evil&&), "");
static_assert(__is_trivially_destructible(T23a::Evil), "");
static_assert(!__is_trivially_relocatable(T23a::Evil), "despite being trivially move-constructible and trivially destructible, it has a user-provided copy constructor");
static_assert(__is_trivially_constructible(T23a, T23a&&), "");
static_assert(__is_trivially_destructible(T23a), "");
static_assert(!__is_trivially_relocatable(T23a), "because it has a non-trivially relocatable member");

struct T23b {
    struct Evil {
        Evil(Evil&) = delete; // expected-note{{explicitly marked deleted here}}
        Evil(const Evil&) = default;
        ~Evil() = default;
    };
    mutable Evil m; // expected-note{{implicitly deleted because}}
    T23b(const T23b&) = default;  // no implicit move constructor
    // expected-warning@-1{{explicitly defaulted copy constructor is implicitly deleted}}
    // expected-note@-2{{replace 'default' with 'delete'}}
};
static_assert(__is_trivially_constructible(T23b::Evil, T23b::Evil&&), "");
static_assert(__is_trivially_destructible(T23b::Evil), "");
static_assert(__is_trivially_relocatable(T23b::Evil), "it has no user-provided copy constructors");
static_assert(!__is_constructible(T23b, T23b&&), "");
static_assert(!__is_trivially_relocatable(T23b), "because it is not move-constructible");


// Verify that the attribute is not inappropriately inherited by derived classes.
struct [[clang::trivially_relocatable]] T24 {
    int i;
    T24(int);
    T24(const T24&);
};
static_assert(__is_trivially_relocatable(T24), "it has the attribute");
struct T24a : public T24 {
    using T24::T24;
};
static_assert(__is_trivially_relocatable(T24a), "all its bases are trivially relocatable");
struct T24b : public T24 {
    T24b(int i) : T24(i) {}
    T24b(const T24b&);
};
static_assert(!__is_trivially_relocatable(T24b), "it redefines the copy constructor");
struct T24c : public T24 {
    T24c(int i) : T24(i) {}
    T24c(const T24c&) = default;
};
static_assert(__is_trivially_relocatable(T24c), "its redefined copy constructor is merely defaulted");
struct T24d : public T24 {
    T24d(int i) : T24(i) {}
    T24d(const T24d&);
};
T24d::T24d(const T24d&) = default;
static_assert(!__is_trivially_relocatable(T24d), "its redefined copy constructor is not defaulted on first declaration");

// Example that regressed with a version of Mark de Wever's patch.
// T25 is analogous to unique_ptr; T25c is analogous to __compressed_pair.
template<class T>
struct [[clang::trivially_relocatable]] T25 {
    T25(T25&&);
};
template<class T> struct T25b { T m_a; };
template<class T> struct T25c : T25b<T> {};
static_assert(__is_trivially_relocatable(T25<int>), "it has the attribute");
static_assert(__is_trivially_relocatable(T25b<int>), "all its members are trivially relocatable");
static_assert(__is_trivially_relocatable(T25c<int>), "all its bases are trivially relocatable");
static_assert(__is_trivially_relocatable(T25b<T25<int>>), "all its members are trivially relocatable");
static_assert(__is_trivially_relocatable(T25c<T25<int>>), "all its bases are trivially relocatable");

// Example from D1144R0
struct string {
    char *data_;
    unsigned long size_ = 0;
    unsigned long capacity_ = 0;
    string() = default;
    string(const char *s);
    string(string&& s);
    ~string();
};
static_assert(!__is_trivially_relocatable(string), "");

// Example from D1144R0
struct offset_ptr {
    unsigned long value_;
    offset_ptr();
    offset_ptr(void *p);
    offset_ptr(const offset_ptr& rhs);
    offset_ptr& operator=(const offset_ptr& rhs);
    ~offset_ptr() = default;
};
static_assert(!__is_trivially_relocatable(offset_ptr), "");

// Example from D1144R0
struct registered_object {
    registered_object();
    registered_object(registered_object&&) = default;
    registered_object(const registered_object&) = default;
    registered_object& operator=(registered_object&&) = default;
    registered_object& operator=(const registered_object&) = default;
    ~registered_object();
};
struct Widget : registered_object {};
static_assert(!__is_trivially_relocatable(registered_object), "");
static_assert(!__is_trivially_relocatable(Widget), "");

// Examples from D1144R0 draft revision 11
namespace ND11 {
    struct M {
        M() = default;
        M(M&);
        M(const M&) = default;
    };
    static_assert( __is_trivially_constructible(M, M&&), "" );
    static_assert( __is_trivially_destructible(M), "" );
    static_assert( !__is_trivially_relocatable(M), "" );

    struct N {
        mutable M m;
    };
    static_assert( __is_trivially_constructible(N, N&&), "" );
    static_assert( __is_trivially_destructible(N), "" );
    static_assert( !__is_trivially_relocatable(N), "" );

    struct [[clang::trivially_relocatable]] O {
        O(const O&);
        mutable int o;
    };
    static_assert( __is_trivially_relocatable(O), "" );

    struct T : N {
        T(const T&) = default;
    };
    static_assert( !__is_trivially_constructible(T, T&&), "" );
    static_assert( __is_trivially_destructible(T), "" );
    static_assert( !__is_trivially_relocatable(T), "" );

    struct U : N {};
    static_assert( __is_trivially_constructible(U, U&&), "" );
    static_assert( __is_trivially_destructible(U), "" );
    static_assert( !__is_trivially_relocatable(U), "" );

    struct V {
        O o;
    };
    static_assert( __is_trivially_relocatable(V), "" );

    struct W {
        O o;
        W(const W&) = default;
    };
    static_assert( __is_trivially_relocatable(W), "" );
} // namespace ND11

// Examples from P1144R0 (still present in P1144R6)
namespace ND14 {
    struct A {
        struct MA {
            MA(MA&);
            MA(const MA&) = default;
            MA(MA&&) = default;
        };
        mutable MA ma;
        A(const A&) = default;
    };
    static_assert(!__is_trivially_relocatable(A), "calls user-provided MA(MA&)");

    struct B {
        struct MB {
            MB(const volatile MB&);
            MB(const MB&) = default;
            MB(MB&&) = default;
        };
        volatile MB mb;
        B(const B&) = default;
    };
    static_assert(!__is_trivially_relocatable(B), "calls user-provided MB(const volatile MB&)");

    struct [[clang::trivially_relocatable]] I {
        I(I&&);
    };
    struct J : I {
        J(const J&);
        J(J&&) = default;
    };
    static_assert(__is_trivially_relocatable(I), "has the attribute");
    static_assert(!__is_trivially_relocatable(J), "inheritance pattern formerly used by std::vector etc.");

    struct [[clang::trivially_relocatable(__is_trivially_relocatable(I))]] J2 : I {
        J2(const J2&);
        J2(J2&&) = default;
    };
    static_assert(__is_trivially_relocatable(J2), "inheritance pattern currently used by std::vector etc.");

    struct [[clang::trivially_relocatable]] K {
        K(const K&&);
        K(const K&);
        K(K&&);
        K(K&);
        volatile int m1;
        mutable int m2;
        ~K();
    };
    struct L : K {
        K k;
    };
    static_assert(__is_trivially_relocatable(K), "the attribute should override all other considerations");
    static_assert(__is_trivially_relocatable(L), "the Rule of Zero should work as expected");
}

// Example from Nicolas Lesser
struct NL1 {
    NL1& operator=(NL1&&);
};
static_assert(!__is_trivially_relocatable(NL1), "");

struct [[clang::trivially_relocatable]] NL2 {
// expected-error@-1{{cannot be applied to struct 'NL2' because it is not move-constructible}}
    NL2& operator=(NL2&&);
};
static_assert(!__is_trivially_relocatable(NL2), "");

union [[clang::trivially_relocatable]] NL3 {
// expected-error@-1{{cannot be applied to union 'NL3' because it is not destructible}}
    struct [[clang::trivially_relocatable]] String { String(String&&); ~String(); };
    int i;
    String s;
};
static_assert(!__is_trivially_relocatable(NL3), "");

union [[clang::trivially_relocatable]] NL4 {
    struct [[clang::trivially_relocatable]] String { String(String&&); ~String(); };
    int i;
    String s;
    NL4(const NL4&);
    ~NL4();
};
static_assert(__is_trivially_relocatable(NL4), "");

template<class T>
struct [[clang::trivially_relocatable]] NL5 {
    T t;
};
struct NL5a {
    NL5a() = default;
    NL5a(NL5a&&) = delete;
};
struct NL5b {
    NL5b() = default;
    NL5b(NL5b&&);
};
static_assert(!__is_trivially_relocatable(NL5<NL5a>), "");
static_assert(__is_trivially_relocatable(NL5<NL5b>), "");

struct NL6 {
    NL6(volatile NL6&) = delete;
    NL6(const NL6&) = default;
};
static_assert(__is_trivially_constructible(NL6, NL6&&), "");
static_assert(__is_trivially_destructible(NL6), "");
static_assert(__is_trivially_relocatable(NL6), "it is trivially move-constructible and trivially destructible");

struct CantPassInRegisters {
  CantPassInRegisters(const CantPassInRegisters&) = delete;
};
static_assert(!__is_trivially_relocatable(CantPassInRegisters), "not move-constructible");
