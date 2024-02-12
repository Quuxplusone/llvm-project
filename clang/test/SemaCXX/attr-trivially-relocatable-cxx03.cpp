// RUN: %clang_cc1 -fsyntax-only -verify %s -std=c++03
// RUN: %clang_cc1 -fsyntax-only -verify %s -std=c++11
// RUN: %clang_cc1 -fsyntax-only -verify %s -std=c++26
// expected-no-diagnostics

#if __cplusplus < 201101L
 #define static_assert _Static_assert
#endif

// It shall appear at most once in each attribute-list
// and no attribute-argument-clause shall be present.

struct [[clang::trivially_relocatable, clang::trivially_relocatable]] B1 {}; // should really be an error

struct [[clang::trivially_relocatable]] [[clang::trivially_relocatable]] B2 {}; // should really be an error

struct [[clang::trivially_relocatable(42)]] B3 {}; // should really be an error, since it's not dependent

template<class T> struct [[clang::trivially_relocatable(T(1))]] B3b {};
B3b<bool> b3b_bool; // OK
B3b<int> b3b_int; // should probably warn, but probably not be an error

// Test alternative spellings.
namespace N {
  struct [[clang::trivially_relocatable]] S1 { ~S1(); };
  struct [[_Clang::trivially_relocatable]] S2 { ~S2(); };
  struct [[_Clang::__trivially_relocatable__]] S3 { ~S3(); };
  struct __attribute__((trivially_relocatable)) S4 { ~S4(); };
  struct __attribute__((__trivially_relocatable__)) S5 { ~S5(); };
  struct Control { ~Control(); };
  static_assert(__is_trivially_relocatable(S1), "");
  static_assert(__is_trivially_relocatable(S2), "");
  static_assert(__is_trivially_relocatable(S3), "");
  static_assert(__is_trivially_relocatable(S4), "");
  static_assert(__is_trivially_relocatable(S5), "");
  static_assert(!__is_trivially_relocatable(Control), "");
}

// Verify that the attribute is not inappropriately inherited by derived classes.
struct __attribute__((trivially_relocatable)) T24 {
    int i;
    T24(int);
    T24(const T24&);
};
static_assert(__is_trivially_relocatable(T24), "it has the attribute");
struct T24a : public T24 {
    T24a(int);
};
static_assert(__is_trivially_relocatable(T24a), "all its bases are trivially relocatable");
struct T24b : public T24 {
    T24b(int i) : T24(i) {}
    T24b(const T24b&);
};
static_assert(!__is_trivially_relocatable(T24b), "it redefines the copy constructor");

namespace AttrFalse {
  struct [[clang::trivially_relocatable(false)]] StillTC {
    int i;
  };
  static_assert(__is_trivially_copyable(StillTC), "");
  static_assert(__is_trivially_relocatable(StillTC), "attr(false) doesn't override triviality");

  struct [[clang::trivially_relocatable(true)]] TR {
    int i;
    TR(const TR&);
    TR& operator=(const TR&);
    ~TR();
  };
  static_assert(__is_trivially_relocatable(TR), "");

  struct [[clang::trivially_relocatable(false)]] StillTR {
    StillTC tc;
    TR tr;
  };
  static_assert(!__is_trivially_copyable(StillTR), "");
  static_assert(__is_trivially_relocatable(StillTR), "attr(false) doesn't override triviality");

  template<class T>
  struct [[clang::trivially_relocatable(false)]] NotTR {
    T t;
    ~NotTR();
  };
  static_assert(!__is_trivially_relocatable(NotTR<int>), "");
  static_assert(!__is_trivially_relocatable(NotTR<int[2]>), "");

  static_assert(sizeof(TR) == sizeof(int), "test the test");
  static_assert(sizeof(StillTR) != sizeof(int), "test the test");
  static_assert(sizeof(NotTR<int>) == sizeof(int), "test the test");
  static_assert(sizeof(NotTR<int[2]>) != sizeof(int), "test the test");

  template <class T>
  struct [[clang::trivially_relocatable(sizeof(T) == sizeof(int))]] Conditional {
    T t;
  };
  static_assert(__is_trivially_relocatable(Conditional<int>), "");
  static_assert(__is_trivially_relocatable(Conditional<int[2]>), "");
  static_assert(__is_trivially_relocatable(Conditional<TR>), "");
  static_assert(__is_trivially_relocatable(Conditional<StillTR>), "");
  static_assert(__is_trivially_relocatable(Conditional<NotTR<int> >), "because NotTR<int> has the right size");
  static_assert(!__is_trivially_relocatable(Conditional<NotTR<int[2]> >), "");
}

namespace ND11 {
  struct __attribute__((trivially_relocatable)) O {
    O(const O&);
    mutable int o;
  };
  static_assert(__is_trivially_relocatable(O), "");

  struct V : O {};
  static_assert(__is_trivially_relocatable(V), "");

  struct J : O {
    J(const J&);
  };
  static_assert(!__is_trivially_relocatable(J), "");

  struct __attribute__((trivially_relocatable(__is_trivially_relocatable(O)))) J2 : O {
    J2(const J2&);
    ~J2();
  };
  static_assert(__is_trivially_relocatable(J2), "");

  template<bool B>
  struct __attribute__((trivially_relocatable(!B))) J3 {
    ~J3();
  };
  static_assert(!__is_trivially_relocatable(J3<true>), "");
  static_assert(__is_trivially_relocatable(J3<false>), "");
}

struct [[clang::trivially_relocatable]] NL2 {
    NL2& operator=(const NL2&);
};
static_assert(__is_trivially_relocatable(NL2), "");

union __attribute__((trivially_relocatable)) U {
  int i;
  const char *p;
  U();
  U(const U&);
  U& operator=(const U&);
  ~U();
};
static_assert(__is_trivially_relocatable(U), "");
