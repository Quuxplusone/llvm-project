// RUN: %clang_cc1 -verify %s

struct X {
  int a; // expected-note {{previous}}
  void b(); // expected-note {{previous}}
  struct c; // expected-note {{previous}}
  typedef int d; // expected-note {{previous}}

  union {
    int a; // expected-error {{member of anonymous union redeclares}}
    int b; // expected-error {{member of anonymous union redeclares}}
    int c; // expected-error {{member of anonymous union redeclares}}
    int d; // expected-error {{member of anonymous union redeclares}}
    int e; // expected-note {{previous}}
    int f; // expected-note {{previous}}
    int g; // expected-note {{previous}}
    int h; // expected-note {{previous}}
  };

  int e; // expected-error {{duplicate member}}
  void f(); // expected-error {{redefinition}}
  struct g; // expected-error {{redefinition}}
  typedef int h; // expected-error {{redefinition}}
};

struct Y {
  union {
    int i;
    static int s; // expected-error {{static members cannot be declared in an anonymous union}}
    // expected-warning@-1{{static data member 's' not allowed in unnamed union}}
  };
};

void x() {
  int a; // expected-note {{previous}}
  void b(); // expected-note {{previous}}
  struct c; // expected-note {{previous}}
  typedef int d; // expected-note {{previous}}

  union {
    int a; // expected-error {{member of anonymous union redeclares}}
    int b; // expected-error {{member of anonymous union redeclares}}
    int c; // expected-error {{member of anonymous union redeclares}}
    int d; // expected-error {{member of anonymous union redeclares}}
    int e; // expected-note {{previous}}
    int f; // expected-note {{previous}}
    int g; // expected-note {{previous}}
    int h; // expected-note {{previous}}
  };

  int e; // expected-error {{redefinition of 'e'}}
  void f(); // expected-error {{redefinition of 'f'}}
  struct g; // expected-error {{redefinition of 'g'}}
  typedef int h; // expected-error {{redefinition of 'h'}}
}

void y() {
  union {
    int i;
    static int s; // expected-error {{static members cannot be declared in an anonymous union}}
    // expected-warning@-1{{static data member 's' not allowed in unnamed union}}
  };
}
