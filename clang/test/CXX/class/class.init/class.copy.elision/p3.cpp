// RUN: %clang_cc1 -std=c++20 -fsyntax-only -fcxx-exceptions -verify=cxx20 %s
// RUN: %clang_cc1 -std=c++17 -fsyntax-only -fcxx-exceptions -verify=cxx11_14_17 %s
// RUN: %clang_cc1 -std=c++14 -fsyntax-only -fcxx-exceptions -verify=cxx11_14_17 %s
// RUN: %clang_cc1 -std=c++11 -fsyntax-only -fcxx-exceptions -verify=cxx11_14_17 %s

// In C++20, implicitly movable entity can be rvalue reference to non-volatile
// automatic object.
namespace test_implicitly_movable_rvalue_ref {
class C {
public:
  C(C &&);
  C(const C &) = delete; // cxx11_14_17-note {{'C' has been explicitly marked deleted here}}
};

C test(C &&c) {
  return c; // cxx11_14_17-error {{call to deleted constructor of 'test_implicitly_movable_rvalue_ref::C'}}
}
} // namespace test_implicitly_movable_rvalue_ref

// In C++20, operand of throw-expression can be function parameter or
// catch-clause parameter.
namespace test_throw_parameter {
class C {
public:
  C(const C &);

private:
  C(C &&); // cxx20-note {{declared private here}}
};

void func();
void test() {
  try {
    func();
  } catch (C c) {
    throw c; // cxx20-error {{calling a private constructor of class 'test_throw_parameter::C'}}
  }
}
} // namespace test_throw_parameter

// In C++20, during the first overload resolution, the selected function no
// need to be a constructor.
namespace test_non_ctor_conversion {
class C {};

class Converter {
public:
  operator C() const & = delete; // cxx11_14_17-note {{'operator C' has been explicitly marked deleted here}}
  operator C() &&;
};

C test() {
  Converter c;
  return c; // cxx11_14_17-error {{conversion function from 'test_non_ctor_conversion::Converter' to 'test_non_ctor_conversion::C' invokes a deleted function}}
}
} // namespace test_non_ctor_conversion

// In C++20, during the first overload resolution, the first parameter of the
// selected function no need to be an rvalue reference to the object's type.
namespace test_ctor_param_rvalue_ref {
class B {
public:
  B();
  B(B &&);
  B(const B &) = delete; // cxx11_14_17-note {{'B' has been explicitly marked deleted here}}
};

class NeedRvalueRef {
public:
  NeedRvalueRef(B &&);
};

class NeedValue {
public:
  NeedValue(B); // cxx11_14_17-note {{passing argument to parameter here}}
};

NeedRvalueRef test1() {
  B b;
  return b;
}

NeedValue test2() {
  B b;
  return b; // cxx11_14_17-error {{call to deleted constructor of 'test_ctor_param_rvalue_ref::B'}}
}
} // namespace test_ctor_param_rvalue_ref
