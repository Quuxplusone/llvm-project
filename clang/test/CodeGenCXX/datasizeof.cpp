// RUN: %clang_cc1 -triple x86_64 -emit-llvm %s -o - | FileCheck %s

// CHECK:      define dso_local noundef i32 @_Z4testi(i32 noundef %i) #0 {
// CHECK-NEXT: entry:
// CHECK-NEXT:   %i.addr = alloca i32, align 4
// CHECK-NEXT:   store i32 %i, ptr %i.addr, align 4
// CHECK-NEXT:   %0 = load i32, ptr %i.addr, align 4
// CHECK-NEXT:   %inc = add nsw i32 %0, 1
// CHECK-NEXT:   store i32 %inc, ptr %i.addr, align 4
// CHECK-NEXT:   %1 = zext i32 %0 to i64
// CHECK-NEXT:   %2 = mul nuw i64 4, %1
// CHECK-NEXT:   %3 = load i32, ptr %i.addr, align 4
// CHECK-NEXT:   ret i32 %3
// CHECK-NEXT: }

int test(int i) {
  (void)__datasizeof(int[i++]);
  return i;
}
