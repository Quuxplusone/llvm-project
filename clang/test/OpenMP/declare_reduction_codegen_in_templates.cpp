// RUN: %clang_cc1 -verify -fopenmp -x c++ -std=c++17 -emit-llvm %s -triple x86_64-linux -fexceptions -fcxx-exceptions -o - -femit-all-decls -disable-llvm-passes | FileCheck %s
// RUN: %clang_cc1 -fopenmp -x c++ -std=c++17 -triple x86_64-linux -fexceptions -fcxx-exceptions -emit-pch -o %t %s -femit-all-decls -disable-llvm-passes
// RUN: %clang_cc1 -fopenmp -x c++ -triple x86_64-linux -fexceptions -fcxx-exceptions -std=c++17 -include-pch %t -verify %s -emit-llvm -o - -femit-all-decls -disable-llvm-passes | FileCheck %s

// RUN: %clang_cc1 -verify -fopenmp-simd -x c++ -std=c++17 -emit-llvm %s -triple x86_64-linux -fexceptions -fcxx-exceptions -o - -femit-all-decls -disable-llvm-passes | FileCheck --check-prefix SIMD-ONLY0 %s
// RUN: %clang_cc1 -fopenmp-simd -x c++ -std=c++17 -triple x86_64-linux -fexceptions -fcxx-exceptions -emit-pch -o %t %s -femit-all-decls -disable-llvm-passes
// RUN: %clang_cc1 -fopenmp-simd -x c++ -triple x86_64-linux -fexceptions -fcxx-exceptions -std=c++17 -include-pch %t -verify %s -emit-llvm -o - -femit-all-decls -disable-llvm-passes | FileCheck --check-prefix SIMD-ONLY0 %s
// SIMD-ONLY0-NOT: {{__kmpc|__tgt}}
// expected-no-diagnostics

// CHECK: call void (ptr, i32, ptr, ...) @__kmpc_fork_call(ptr @{{.+}}, i32 1, ptr [[OUTLINED:@.+]], ptr %{{.+}})

// CHECK: define internal void [[OUTLINED]](ptr noalias noundef %{{.+}}, ptr noalias noundef %{{.+}}, ptr {{.+}})
// CHECK: call i32 @__kmpc_reduce_nowait(ptr

#ifndef HEADER
#define HEADER

typedef decltype(sizeof 0) a;
namespace std {
template <class> class initializer_list {
  const int *b;
  a c;
};
template <typename, typename> class d {};
template <typename e> class f {
public:
  f(initializer_list<e>);
};
} // namespace std
template <class g, class h> void foo(g, h) {
  std::d<a, double> i;
#pragma omp declare reduction(j : std::d <a, double> : []{}())
#pragma omp parallel reduction(j : i)
  ;
}
void k() {
  std::f<int> l{};
  std::f<int> m{2};
  foo(l, m);
}

#endif // HEADER
