; NOTE: Assertions have been autogenerated by utils/update_mir_test_checks.py
; RUN: llc -mtriple=riscv32 -mattr=+v,+zvfbfmin,+zvfh -global-isel -stop-after=irtranslator \
; RUN:   -verify-machineinstrs < %s | FileCheck -check-prefixes=RV32 %s
; RUN: llc -mtriple=riscv64 -mattr=+v,+zvfbfmin,+zvfh -global-isel -stop-after=irtranslator \
; RUN:   -verify-machineinstrs < %s | FileCheck -check-prefixes=RV64 %s

; ==========================================================================
; ============================= Scalable Types =============================
; ==========================================================================

define <vscale x 1 x i8> @test_ret_nxv1i8() {
  ; RV32-LABEL: name: test_ret_nxv1i8
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s8>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s8>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv1i8
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s8>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s8>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 1 x i8> undef
}

define <vscale x 2 x i8> @test_ret_nxv2i8() {
  ; RV32-LABEL: name: test_ret_nxv2i8
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s8>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s8>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv2i8
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s8>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s8>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 2 x i8> undef
}

define <vscale x 4 x i8> @test_ret_nxv4i8() {
  ; RV32-LABEL: name: test_ret_nxv4i8
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s8>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 4 x s8>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv4i8
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s8>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 4 x s8>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 4 x i8> undef
}

define <vscale x 8 x i8> @test_ret_nxv8i8() {
  ; RV32-LABEL: name: test_ret_nxv8i8
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s8>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 8 x s8>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv8i8
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s8>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 8 x s8>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 8 x i8> undef
}

define <vscale x 16 x i8> @test_ret_nxv16i8() {
  ; RV32-LABEL: name: test_ret_nxv16i8
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s8>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 16 x s8>)
  ; RV32-NEXT:   PseudoRET implicit $v8m2
  ;
  ; RV64-LABEL: name: test_ret_nxv16i8
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s8>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 16 x s8>)
  ; RV64-NEXT:   PseudoRET implicit $v8m2
entry:
  ret <vscale x 16 x i8> undef
}

define <vscale x 32 x i8> @test_ret_nxv32i8() {
  ; RV32-LABEL: name: test_ret_nxv32i8
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s8>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 32 x s8>)
  ; RV32-NEXT:   PseudoRET implicit $v8m4
  ;
  ; RV64-LABEL: name: test_ret_nxv32i8
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s8>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 32 x s8>)
  ; RV64-NEXT:   PseudoRET implicit $v8m4
entry:
  ret <vscale x 32 x i8> undef
}

define <vscale x 64 x i8> @test_ret_nxv64i8() {
  ; RV32-LABEL: name: test_ret_nxv64i8
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 64 x s8>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 64 x s8>)
  ; RV32-NEXT:   PseudoRET implicit $v8m8
  ;
  ; RV64-LABEL: name: test_ret_nxv64i8
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 64 x s8>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 64 x s8>)
  ; RV64-NEXT:   PseudoRET implicit $v8m8
entry:
  ret <vscale x 64 x i8> undef
}

define <vscale x 1 x i16> @test_ret_nxv1i16() {
  ; RV32-LABEL: name: test_ret_nxv1i16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv1i16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 1 x i16> undef
}

define <vscale x 2 x i16> @test_ret_nxv2i16() {
  ; RV32-LABEL: name: test_ret_nxv2i16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv2i16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 2 x i16> undef
}

define <vscale x 4 x i16> @test_ret_nxv4i16() {
  ; RV32-LABEL: name: test_ret_nxv4i16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 4 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv4i16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 4 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 4 x i16> undef
}

define <vscale x 8 x i16> @test_ret_nxv8i16() {
  ; RV32-LABEL: name: test_ret_nxv8i16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 8 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m2
  ;
  ; RV64-LABEL: name: test_ret_nxv8i16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 8 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m2
entry:
  ret <vscale x 8 x i16> undef
}

define <vscale x 16 x i16> @test_ret_nxv16i16() {
  ; RV32-LABEL: name: test_ret_nxv16i16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 16 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m4
  ;
  ; RV64-LABEL: name: test_ret_nxv16i16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 16 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m4
entry:
  ret <vscale x 16 x i16> undef
}

define <vscale x 32 x i16> @test_ret_nxv32i16() {
  ; RV32-LABEL: name: test_ret_nxv32i16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 32 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m8
  ;
  ; RV64-LABEL: name: test_ret_nxv32i16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 32 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m8
entry:
  ret <vscale x 32 x i16> undef
}

define <vscale x 1 x i32> @test_ret_nxv1i32() {
  ; RV32-LABEL: name: test_ret_nxv1i32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv1i32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 1 x i32> undef
}

define <vscale x 2 x i32> @test_ret_nxv2i32() {
  ; RV32-LABEL: name: test_ret_nxv2i32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv2i32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 2 x i32> undef
}

define <vscale x 4 x i32> @test_ret_nxv4i32() {
  ; RV32-LABEL: name: test_ret_nxv4i32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 4 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8m2
  ;
  ; RV64-LABEL: name: test_ret_nxv4i32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 4 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8m2
entry:
  ret <vscale x 4 x i32> undef
}

define <vscale x 8 x i32> @test_ret_nxv8i32() {
  ; RV32-LABEL: name: test_ret_nxv8i32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 8 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8m4
  ;
  ; RV64-LABEL: name: test_ret_nxv8i32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 8 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8m4
entry:
  ret <vscale x 8 x i32> undef
}

define <vscale x 16 x i32> @test_ret_nxv16i32() {
  ; RV32-LABEL: name: test_ret_nxv16i32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 16 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8m8
  ;
  ; RV64-LABEL: name: test_ret_nxv16i32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 16 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8m8
entry:
  ret <vscale x 16 x i32> undef
}

define <vscale x 1 x i64> @test_ret_nxv1i64() {
  ; RV32-LABEL: name: test_ret_nxv1i64
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s64>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s64>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv1i64
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s64>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s64>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 1 x i64> undef
}

define <vscale x 2 x i64> @test_ret_nxv2i64() {
  ; RV32-LABEL: name: test_ret_nxv2i64
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s64>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 2 x s64>)
  ; RV32-NEXT:   PseudoRET implicit $v8m2
  ;
  ; RV64-LABEL: name: test_ret_nxv2i64
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s64>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 2 x s64>)
  ; RV64-NEXT:   PseudoRET implicit $v8m2
entry:
  ret <vscale x 2 x i64> undef
}

define <vscale x 4 x i64> @test_ret_nxv4i64() {
  ; RV32-LABEL: name: test_ret_nxv4i64
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s64>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 4 x s64>)
  ; RV32-NEXT:   PseudoRET implicit $v8m4
  ;
  ; RV64-LABEL: name: test_ret_nxv4i64
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s64>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 4 x s64>)
  ; RV64-NEXT:   PseudoRET implicit $v8m4
entry:
  ret <vscale x 4 x i64> undef
}

define <vscale x 8 x i64> @test_ret_nxv8i64() {
  ; RV32-LABEL: name: test_ret_nxv8i64
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s64>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 8 x s64>)
  ; RV32-NEXT:   PseudoRET implicit $v8m8
  ;
  ; RV64-LABEL: name: test_ret_nxv8i64
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s64>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 8 x s64>)
  ; RV64-NEXT:   PseudoRET implicit $v8m8
entry:
  ret <vscale x 8 x i64> undef
}

define <vscale x 64 x i1> @test_ret_nxv64i1() {
  ; RV32-LABEL: name: test_ret_nxv64i1
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 64 x s1>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v0 = COPY [[DEF]](<vscale x 64 x s1>)
  ; RV32-NEXT:   PseudoRET implicit $v0
  ;
  ; RV64-LABEL: name: test_ret_nxv64i1
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 64 x s1>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v0 = COPY [[DEF]](<vscale x 64 x s1>)
  ; RV64-NEXT:   PseudoRET implicit $v0
entry:
  ret <vscale x 64 x i1> undef
}

define <vscale x 32 x i1> @test_ret_nxv32i1() {
  ; RV32-LABEL: name: test_ret_nxv32i1
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s1>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v0 = COPY [[DEF]](<vscale x 32 x s1>)
  ; RV32-NEXT:   PseudoRET implicit $v0
  ;
  ; RV64-LABEL: name: test_ret_nxv32i1
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s1>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v0 = COPY [[DEF]](<vscale x 32 x s1>)
  ; RV64-NEXT:   PseudoRET implicit $v0
entry:
  ret <vscale x 32 x i1> undef
}

define <vscale x 16 x i1> @test_ret_nxv16i1() {
  ; RV32-LABEL: name: test_ret_nxv16i1
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s1>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v0 = COPY [[DEF]](<vscale x 16 x s1>)
  ; RV32-NEXT:   PseudoRET implicit $v0
  ;
  ; RV64-LABEL: name: test_ret_nxv16i1
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s1>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v0 = COPY [[DEF]](<vscale x 16 x s1>)
  ; RV64-NEXT:   PseudoRET implicit $v0
entry:
  ret <vscale x 16 x i1> undef
}

define <vscale x 8 x i1> @test_ret_nxv8i1() {
  ; RV32-LABEL: name: test_ret_nxv8i1
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s1>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v0 = COPY [[DEF]](<vscale x 8 x s1>)
  ; RV32-NEXT:   PseudoRET implicit $v0
  ;
  ; RV64-LABEL: name: test_ret_nxv8i1
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s1>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v0 = COPY [[DEF]](<vscale x 8 x s1>)
  ; RV64-NEXT:   PseudoRET implicit $v0
entry:
  ret <vscale x 8 x i1> undef
}

define <vscale x 4 x i1> @test_ret_nxv4i1() {
  ; RV32-LABEL: name: test_ret_nxv4i1
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s1>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v0 = COPY [[DEF]](<vscale x 4 x s1>)
  ; RV32-NEXT:   PseudoRET implicit $v0
  ;
  ; RV64-LABEL: name: test_ret_nxv4i1
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s1>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v0 = COPY [[DEF]](<vscale x 4 x s1>)
  ; RV64-NEXT:   PseudoRET implicit $v0
entry:
  ret <vscale x 4 x i1> undef
}

define <vscale x 2 x i1> @test_ret_nxv2i1() {
  ; RV32-LABEL: name: test_ret_nxv2i1
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s1>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v0 = COPY [[DEF]](<vscale x 2 x s1>)
  ; RV32-NEXT:   PseudoRET implicit $v0
  ;
  ; RV64-LABEL: name: test_ret_nxv2i1
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s1>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v0 = COPY [[DEF]](<vscale x 2 x s1>)
  ; RV64-NEXT:   PseudoRET implicit $v0
entry:
  ret <vscale x 2 x i1> undef
}

define <vscale x 1 x i1> @test_ret_nxv1i1() {
  ; RV32-LABEL: name: test_ret_nxv1i1
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s1>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v0 = COPY [[DEF]](<vscale x 1 x s1>)
  ; RV32-NEXT:   PseudoRET implicit $v0
  ;
  ; RV64-LABEL: name: test_ret_nxv1i1
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s1>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v0 = COPY [[DEF]](<vscale x 1 x s1>)
  ; RV64-NEXT:   PseudoRET implicit $v0
entry:
  ret <vscale x 1 x i1> undef
}

define <vscale x 1 x float> @test_ret_nxv1f32() {
  ; RV32-LABEL: name: test_ret_nxv1f32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv1f32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 1 x float> undef
}

define <vscale x 2 x float> @test_ret_nxv2f32() {
  ; RV32-LABEL: name: test_ret_nxv2f32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv2f32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 2 x float> undef
}

define <vscale x 4 x float> @test_ret_nxv4f32() {
  ; RV32-LABEL: name: test_ret_nxv4f32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 4 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8m2
  ;
  ; RV64-LABEL: name: test_ret_nxv4f32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 4 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8m2
entry:
  ret <vscale x 4 x float> undef
}

define <vscale x 8 x float> @test_ret_nxv8f32() {
  ; RV32-LABEL: name: test_ret_nxv8f32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 8 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8m4
  ;
  ; RV64-LABEL: name: test_ret_nxv8f32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 8 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8m4
entry:
  ret <vscale x 8 x float> undef
}

define <vscale x 16 x float> @test_ret_nxv16f32() {
  ; RV32-LABEL: name: test_ret_nxv16f32
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s32>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 16 x s32>)
  ; RV32-NEXT:   PseudoRET implicit $v8m8
  ;
  ; RV64-LABEL: name: test_ret_nxv16f32
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s32>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 16 x s32>)
  ; RV64-NEXT:   PseudoRET implicit $v8m8
entry:
  ret <vscale x 16 x float> undef
}

define <vscale x 1 x double> @test_ret_nxv1f64() {
  ; RV32-LABEL: name: test_ret_nxv1f64
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s64>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s64>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv1f64
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s64>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s64>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 1 x double> undef
}

define <vscale x 2 x double> @test_ret_nxv2f64() {
  ; RV32-LABEL: name: test_ret_nxv2f64
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s64>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 2 x s64>)
  ; RV32-NEXT:   PseudoRET implicit $v8m2
  ;
  ; RV64-LABEL: name: test_ret_nxv2f64
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s64>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 2 x s64>)
  ; RV64-NEXT:   PseudoRET implicit $v8m2
entry:
  ret <vscale x 2 x double> undef
}

define <vscale x 4 x double> @test_ret_nxv4f64() {
  ; RV32-LABEL: name: test_ret_nxv4f64
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s64>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 4 x s64>)
  ; RV32-NEXT:   PseudoRET implicit $v8m4
  ;
  ; RV64-LABEL: name: test_ret_nxv4f64
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s64>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 4 x s64>)
  ; RV64-NEXT:   PseudoRET implicit $v8m4
entry:
  ret <vscale x 4 x double> undef
}

define <vscale x 8 x double> @test_ret_nxv8f64() {
  ; RV32-LABEL: name: test_ret_nxv8f64
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s64>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 8 x s64>)
  ; RV32-NEXT:   PseudoRET implicit $v8m8
  ;
  ; RV64-LABEL: name: test_ret_nxv8f64
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s64>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 8 x s64>)
  ; RV64-NEXT:   PseudoRET implicit $v8m8
entry:
  ret <vscale x 8 x double> undef
}

define <vscale x 1 x half> @test_ret_nxv1f16() {
  ; RV32-LABEL: name: test_ret_nxv1f16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv1f16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 1 x half> undef
}

define <vscale x 2 x half> @test_ret_nxv2f16() {
  ; RV32-LABEL: name: test_ret_nxv2f16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv2f16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 2 x half> undef
}

define <vscale x 4 x half> @test_ret_nxv4f16() {
  ; RV32-LABEL: name: test_ret_nxv4f16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 4 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv4f16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 4 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 4 x half> undef
}

define <vscale x 8 x half> @test_ret_nxv8f16() {
  ; RV32-LABEL: name: test_ret_nxv8f16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 8 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m2
  ;
  ; RV64-LABEL: name: test_ret_nxv8f16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 8 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m2
entry:
  ret <vscale x 8 x half> undef
}

define <vscale x 16 x half> @test_ret_nxv16f16() {
  ; RV32-LABEL: name: test_ret_nxv16f16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 16 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m4
  ;
  ; RV64-LABEL: name: test_ret_nxv16f16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 16 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m4
entry:
  ret <vscale x 16 x half> undef
}

define <vscale x 32 x half> @test_ret_nxv32f16() {
  ; RV32-LABEL: name: test_ret_nxv32f16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 32 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m8
  ;
  ; RV64-LABEL: name: test_ret_nxv32f16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 32 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m8
entry:
  ret <vscale x 32 x half> undef
}

define <vscale x 1 x bfloat> @test_ret_nxv1b16() {
  ; RV32-LABEL: name: test_ret_nxv1b16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv1b16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 1 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 1 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 1 x bfloat> undef
}

define <vscale x 2 x bfloat> @test_ret_nxv2b16() {
  ; RV32-LABEL: name: test_ret_nxv2b16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv2b16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 2 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 2 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 2 x bfloat> undef
}

define <vscale x 4 x bfloat> @test_ret_nxv4b16() {
  ; RV32-LABEL: name: test_ret_nxv4b16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8 = COPY [[DEF]](<vscale x 4 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8
  ;
  ; RV64-LABEL: name: test_ret_nxv4b16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 4 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8 = COPY [[DEF]](<vscale x 4 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8
entry:
  ret <vscale x 4 x bfloat> undef
}

define <vscale x 8 x bfloat> @test_ret_nxv8b16() {
  ; RV32-LABEL: name: test_ret_nxv8b16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 8 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m2
  ;
  ; RV64-LABEL: name: test_ret_nxv8b16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 8 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m2 = COPY [[DEF]](<vscale x 8 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m2
entry:
  ret <vscale x 8 x bfloat> undef
}

define <vscale x 16 x bfloat> @test_ret_nxv16b16() {
  ; RV32-LABEL: name: test_ret_nxv16b16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 16 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m4
  ;
  ; RV64-LABEL: name: test_ret_nxv16b16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 16 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m4 = COPY [[DEF]](<vscale x 16 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m4
entry:
  ret <vscale x 16 x bfloat> undef
}

define <vscale x 32 x bfloat> @test_ret_nxv32b16() {
  ; RV32-LABEL: name: test_ret_nxv32b16
  ; RV32: bb.1.entry:
  ; RV32-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s16>) = G_IMPLICIT_DEF
  ; RV32-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 32 x s16>)
  ; RV32-NEXT:   PseudoRET implicit $v8m8
  ;
  ; RV64-LABEL: name: test_ret_nxv32b16
  ; RV64: bb.1.entry:
  ; RV64-NEXT:   [[DEF:%[0-9]+]]:_(<vscale x 32 x s16>) = G_IMPLICIT_DEF
  ; RV64-NEXT:   $v8m8 = COPY [[DEF]](<vscale x 32 x s16>)
  ; RV64-NEXT:   PseudoRET implicit $v8m8
entry:
  ret <vscale x 32 x bfloat> undef
}
