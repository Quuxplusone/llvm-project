// RUN: %clang_cc1 -std=c++17 -Wunused-const-variable -fstatic-init-lists -fsyntax-only -verify %s
// RUN: %clang_cc1 -std=c++17 -Wunused-const-variable -fno-static-init-lists -fsyntax-only -verify %s
// expected-no-diagnostics

namespace std {
  typedef decltype(sizeof(int)) size_t;

  // libc++'s implementation
  template <class _E>
  class initializer_list {
    const _E* __begin_;
    size_t    __size_;

    initializer_list(const _E* __b, size_t __s)
      : __begin_(__b),
        __size_(__s)
    {}

  public:
    typedef _E        value_type;
    typedef const _E& reference;
    typedef const _E& const_reference;
    typedef size_t    size_type;

    typedef const _E* iterator;
    typedef const _E* const_iterator;

    constexpr initializer_list() : __begin_(nullptr), __size_(0) {}

    constexpr size_t    size()  const {return __size_;}
    constexpr const _E* begin() const {return __begin_;}
    constexpr const _E* end()   const {return __begin_ + __size_;}
  };
}

void simple(int x) {
  (void)std::initializer_list<int>{ 1, 2, 3 };
  (void)std::initializer_list<int>{ 1, 2, 3 };
  (void)std::initializer_list<int>{ 1, x, 3 };
}
template<class T>
void templated(int x) {
  (void)std::initializer_list<int>{ 1, 2, 3 };
  (void)std::initializer_list<T>{ 1, 2, 3 };
  (void)std::initializer_list<int>{ T(), T(), T() };
  (void)std::initializer_list<T>{ T(), T(), T() };
}
template void templated<int>(int);
template void templated<short>(int);

struct S1 {
  std::initializer_list<int> il_ = {1, 2, 3};
  S1();
};
S1::S1() { }

struct S2 {
  std::initializer_list<int> il_;
  S2();
};
S2::S2() : il_{1, 2, 3} { }

struct A {
  int i_;
  constexpr A(int i) : i_(i) { }
};
void constexpr_construction_test() {
  (void)std::initializer_list<A>{ 1, 2, 3 };
}

constexpr int use_il_in_constexpr(std::initializer_list<int> il) {
  int sum = 0;
  for (int i : il) {
    sum += i;
  }
  return sum;
}
static_assert(use_il_in_constexpr({1,2,3}) == 6);

struct E {
  E(std::initializer_list<int>);
};

auto sfinae_context() -> decltype(E{1,2,3}) {
  return E{1,2,3};
}

template<class T>
auto sfinae_context_templated() -> decltype(T(1), E{1,2,3}) {
  return E{1,2,3};
}

template E sfinae_context_templated<int>();
