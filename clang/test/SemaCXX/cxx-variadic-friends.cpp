// RUN: %clang_cc1 -fsyntax-only -pedantic -verify %s

template <class... Ts>
class Passkey {
  int x = 0;
  friend Ts...;
  Passkey() = default; // expected-note 2 {{implicitly declared private here}}
};

class Blarg;
class Blip;
class Baz;

struct Foo {
  void do_something(Passkey<Blarg, Blip, Baz>);
};

struct Baz {
  void f() {
    Foo f;
    f.do_something(Passkey<Blarg, Blip, Baz>());
  }
};

struct Quack {
  void f() {
    Foo f;
    f.do_something(Passkey<Blarg, Blip, Baz>()); // expected-error {{calling a private constructor of class 'Passkey<Blarg, Blip, Baz>'}}
  }
};

void f() {
  Foo f;
  auto key = Passkey<Blarg, Blip, Baz>(); // expected-error {{calling a private constructor of class 'Passkey<Blarg, Blip, Baz>'}}
  f.do_something(key);
}

class Ta;
struct Tb;
struct Tc;

template<class T> struct Wrapper {};

class CommaSeparatedFriend {
  int f();
  //friend Ta, Tb;
};

template<class... Ts>
class CommaSeparatedFriendWithPack {
  int f();
  //friend Ta, Ts..., Wrapper<Ts>..., Tb;
};

template<class... Ts>
class OnlyTheWrapperPack {
  int f(); // expected-note 6 {{implicitly declared private here}}
  //friend Wrapper<Ts>...;
};

class Ta {
  int f() {
    //CommaSeparatedFriend().f();
    //CommaSeparatedFriendWithPack<Tc>().f();
    OnlyTheWrapperPack<Ta>().f(); // expected-error {{'f' is a private member of}}
    OnlyTheWrapperPack<Ta, Tb>().f(); // expected-error {{'f' is a private member of}}
    OnlyTheWrapperPack<Tb, Ta>().f(); // expected-error {{'f' is a private member of}}
    OnlyTheWrapperPack<Ta, Ta>().f(); // expected-error {{'f' is a private member of}}
  }
};

struct Tb {
  int f() {
    //CommaSeparatedFriend().f();
    //CommaSeparatedFriendWithPack<Tc>().f();
    OnlyTheWrapperPack<Ta, Tb>().f(); // expected-error {{'f' is a private member of}}
  }
};

struct Tc {
  int f() {
    //CommaSeparatedFriend().f(); // not-expected-error {{'f' is a private member of}}
    //CommaSeparatedFriendWithPack<Tc>().f();
  }
};

struct Td {
  int f() {
    //CommaSeparatedFriend().f(); // not-expected-error {{'f' is a private member of}}
    //CommaSeparatedFriendWithPack<Tc>().f(); // not-expected-error {{'f' is a private member of}}
  }
};

template<>
struct Wrapper<Tc> {
  int f() {
    //CommaSeparatedFriend().f(); // not-expected-error {{'f' is a private member of}}
    //CommaSeparatedFriendWithPack<Tc>().f();
    OnlyTheWrapperPack<Ta>().f(); // expected-error {{'f' is a private member of}}
    //OnlyTheWrapperPack<Ta, Tc>().f(); // not-expected-error {{'f' is a private member of}}
    //OnlyTheWrapperPack<Tc, Ta>().f(); // not-expected-error {{'f' is a private member of}}
    //OnlyTheWrapperPack<Tc, Tc>().f(); // not-expected-error {{'f' is a private member of}}
  }
};
