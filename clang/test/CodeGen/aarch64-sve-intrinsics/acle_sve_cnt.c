// RUN: %clang_cc1 -D__ARM_FEATURE_SVE -triple aarch64-none-linux-gnu -target-feature +sve -fallow-half-arguments-and-returns -S -O1 -Werror -Wall -emit-llvm -o - %s | FileCheck %s
// RUN: %clang_cc1 -D__ARM_FEATURE_SVE -DSVE_OVERLOADED_FORMS -triple aarch64-none-linux-gnu -target-feature +sve -fallow-half-arguments-and-returns -S -O1 -Werror -Wall -emit-llvm -o - %s | FileCheck %s

#include <arm_sve.h>

#ifdef SVE_OVERLOADED_FORMS
// A simple used,unused... macro, long enough to represent any SVE builtin.
#define SVE_ACLE_FUNC(A1,A2_UNUSED,A3,A4_UNUSED) A1##A3
#else
#define SVE_ACLE_FUNC(A1,A2,A3,A4) A1##A2##A3##A4
#endif

svuint8_t test_svcnt_s8_z(svbool_t pg, svint8_t op)
{
  // CHECK-LABEL: test_svcnt_s8_z
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 16 x i8> @llvm.aarch64.sve.cnt.nxv16i8(<vscale x 16 x i8> zeroinitializer, <vscale x 16 x i1> %pg, <vscale x 16 x i8> %op)
  // CHECK: ret <vscale x 16 x i8> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s8,_z,)(pg, op);
}

svuint16_t test_svcnt_s16_z(svbool_t pg, svint16_t op)
{
  // CHECK-LABEL: test_svcnt_s16_z
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8i16(<vscale x 8 x i16> zeroinitializer, <vscale x 8 x i1> %[[PG]], <vscale x 8 x i16> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s16,_z,)(pg, op);
}

svuint32_t test_svcnt_s32_z(svbool_t pg, svint32_t op)
{
  // CHECK-LABEL: test_svcnt_s32_z
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4i32(<vscale x 4 x i32> zeroinitializer, <vscale x 4 x i1> %[[PG]], <vscale x 4 x i32> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s32,_z,)(pg, op);
}

svuint64_t test_svcnt_s64_z(svbool_t pg, svint64_t op)
{
  // CHECK-LABEL: test_svcnt_s64_z
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2i64(<vscale x 2 x i64> zeroinitializer, <vscale x 2 x i1> %[[PG]], <vscale x 2 x i64> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s64,_z,)(pg, op);
}

svuint8_t test_svcnt_u8_z(svbool_t pg, svuint8_t op)
{
  // CHECK-LABEL: test_svcnt_u8_z
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 16 x i8> @llvm.aarch64.sve.cnt.nxv16i8(<vscale x 16 x i8> zeroinitializer, <vscale x 16 x i1> %pg, <vscale x 16 x i8> %op)
  // CHECK: ret <vscale x 16 x i8> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u8,_z,)(pg, op);
}

svuint16_t test_svcnt_u16_z(svbool_t pg, svuint16_t op)
{
  // CHECK-LABEL: test_svcnt_u16_z
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8i16(<vscale x 8 x i16> zeroinitializer, <vscale x 8 x i1> %[[PG]], <vscale x 8 x i16> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u16,_z,)(pg, op);
}

svuint32_t test_svcnt_u32_z(svbool_t pg, svuint32_t op)
{
  // CHECK-LABEL: test_svcnt_u32_z
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4i32(<vscale x 4 x i32> zeroinitializer, <vscale x 4 x i1> %[[PG]], <vscale x 4 x i32> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u32,_z,)(pg, op);
}

svuint64_t test_svcnt_u64_z(svbool_t pg, svuint64_t op)
{
  // CHECK-LABEL: test_svcnt_u64_z
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2i64(<vscale x 2 x i64> zeroinitializer, <vscale x 2 x i1> %[[PG]], <vscale x 2 x i64> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u64,_z,)(pg, op);
}

svuint16_t test_svcnt_f16_z(svbool_t pg, svfloat16_t op)
{
  // CHECK-LABEL: test_svcnt_f16_z
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8f16(<vscale x 8 x i16> zeroinitializer, <vscale x 8 x i1> %[[PG]], <vscale x 8 x half> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f16,_z,)(pg, op);
}

svuint32_t test_svcnt_f32_z(svbool_t pg, svfloat32_t op)
{
  // CHECK-LABEL: test_svcnt_f32_z
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4f32(<vscale x 4 x i32> zeroinitializer, <vscale x 4 x i1> %[[PG]], <vscale x 4 x float> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f32,_z,)(pg, op);
}

svuint64_t test_svcnt_f64_z(svbool_t pg, svfloat64_t op)
{
  // CHECK-LABEL: test_svcnt_f64_z
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2f64(<vscale x 2 x i64> zeroinitializer, <vscale x 2 x i1> %[[PG]], <vscale x 2 x double> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f64,_z,)(pg, op);
}

svuint8_t test_svcnt_s8_m(svuint8_t inactive, svbool_t pg, svint8_t op)
{
  // CHECK-LABEL: test_svcnt_s8_m
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 16 x i8> @llvm.aarch64.sve.cnt.nxv16i8(<vscale x 16 x i8> %inactive, <vscale x 16 x i1> %pg, <vscale x 16 x i8> %op)
  // CHECK: ret <vscale x 16 x i8> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s8,_m,)(inactive, pg, op);
}

svuint16_t test_svcnt_s16_m(svuint16_t inactive, svbool_t pg, svint16_t op)
{
  // CHECK-LABEL: test_svcnt_s16_m
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8i16(<vscale x 8 x i16> %inactive, <vscale x 8 x i1> %[[PG]], <vscale x 8 x i16> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s16,_m,)(inactive, pg, op);
}

svuint32_t test_svcnt_s32_m(svuint32_t inactive, svbool_t pg, svint32_t op)
{
  // CHECK-LABEL: test_svcnt_s32_m
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4i32(<vscale x 4 x i32> %inactive, <vscale x 4 x i1> %[[PG]], <vscale x 4 x i32> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s32,_m,)(inactive, pg, op);
}

svuint64_t test_svcnt_s64_m(svuint64_t inactive, svbool_t pg, svint64_t op)
{
  // CHECK-LABEL: test_svcnt_s64_m
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2i64(<vscale x 2 x i64> %inactive, <vscale x 2 x i1> %[[PG]], <vscale x 2 x i64> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s64,_m,)(inactive, pg, op);
}

svuint8_t test_svcnt_u8_m(svuint8_t inactive, svbool_t pg, svuint8_t op)
{
  // CHECK-LABEL: test_svcnt_u8_m
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 16 x i8> @llvm.aarch64.sve.cnt.nxv16i8(<vscale x 16 x i8> %inactive, <vscale x 16 x i1> %pg, <vscale x 16 x i8> %op)
  // CHECK: ret <vscale x 16 x i8> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u8,_m,)(inactive, pg, op);
}

svuint16_t test_svcnt_u16_m(svuint16_t inactive, svbool_t pg, svuint16_t op)
{
  // CHECK-LABEL: test_svcnt_u16_m
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8i16(<vscale x 8 x i16> %inactive, <vscale x 8 x i1> %[[PG]], <vscale x 8 x i16> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u16,_m,)(inactive, pg, op);
}

svuint32_t test_svcnt_u32_m(svuint32_t inactive, svbool_t pg, svuint32_t op)
{
  // CHECK-LABEL: test_svcnt_u32_m
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4i32(<vscale x 4 x i32> %inactive, <vscale x 4 x i1> %[[PG]], <vscale x 4 x i32> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u32,_m,)(inactive, pg, op);
}

svuint64_t test_svcnt_u64_m(svuint64_t inactive, svbool_t pg, svuint64_t op)
{
  // CHECK-LABEL: test_svcnt_u64_m
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2i64(<vscale x 2 x i64> %inactive, <vscale x 2 x i1> %[[PG]], <vscale x 2 x i64> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u64,_m,)(inactive, pg, op);
}

svuint16_t test_svcnt_f16_m(svuint16_t inactive, svbool_t pg, svfloat16_t op)
{
  // CHECK-LABEL: test_svcnt_f16_m
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8f16(<vscale x 8 x i16> %inactive, <vscale x 8 x i1> %[[PG]], <vscale x 8 x half> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f16,_m,)(inactive, pg, op);
}

svuint32_t test_svcnt_f32_m(svuint32_t inactive, svbool_t pg, svfloat32_t op)
{
  // CHECK-LABEL: test_svcnt_f32_m
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4f32(<vscale x 4 x i32> %inactive, <vscale x 4 x i1> %[[PG]], <vscale x 4 x float> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f32,_m,)(inactive, pg, op);
}

svuint64_t test_svcnt_f64_m(svuint64_t inactive, svbool_t pg, svfloat64_t op)
{
  // CHECK-LABEL: test_svcnt_f64_m
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2f64(<vscale x 2 x i64> %inactive, <vscale x 2 x i1> %[[PG]], <vscale x 2 x double> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f64,_m,)(inactive, pg, op);
}

svuint8_t test_svcnt_s8_x(svbool_t pg, svint8_t op)
{
  // CHECK-LABEL: test_svcnt_s8_x
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 16 x i8> @llvm.aarch64.sve.cnt.nxv16i8(<vscale x 16 x i8> undef, <vscale x 16 x i1> %pg, <vscale x 16 x i8> %op)
  // CHECK: ret <vscale x 16 x i8> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s8,_x,)(pg, op);
}

svuint16_t test_svcnt_s16_x(svbool_t pg, svint16_t op)
{
  // CHECK-LABEL: test_svcnt_s16_x
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8i16(<vscale x 8 x i16> undef, <vscale x 8 x i1> %[[PG]], <vscale x 8 x i16> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s16,_x,)(pg, op);
}

svuint32_t test_svcnt_s32_x(svbool_t pg, svint32_t op)
{
  // CHECK-LABEL: test_svcnt_s32_x
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4i32(<vscale x 4 x i32> undef, <vscale x 4 x i1> %[[PG]], <vscale x 4 x i32> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s32,_x,)(pg, op);
}

svuint64_t test_svcnt_s64_x(svbool_t pg, svint64_t op)
{
  // CHECK-LABEL: test_svcnt_s64_x
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2i64(<vscale x 2 x i64> undef, <vscale x 2 x i1> %[[PG]], <vscale x 2 x i64> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_s64,_x,)(pg, op);
}

svuint8_t test_svcnt_u8_x(svbool_t pg, svuint8_t op)
{
  // CHECK-LABEL: test_svcnt_u8_x
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 16 x i8> @llvm.aarch64.sve.cnt.nxv16i8(<vscale x 16 x i8> undef, <vscale x 16 x i1> %pg, <vscale x 16 x i8> %op)
  // CHECK: ret <vscale x 16 x i8> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u8,_x,)(pg, op);
}

svuint16_t test_svcnt_u16_x(svbool_t pg, svuint16_t op)
{
  // CHECK-LABEL: test_svcnt_u16_x
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8i16(<vscale x 8 x i16> undef, <vscale x 8 x i1> %[[PG]], <vscale x 8 x i16> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u16,_x,)(pg, op);
}

svuint32_t test_svcnt_u32_x(svbool_t pg, svuint32_t op)
{
  // CHECK-LABEL: test_svcnt_u32_x
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4i32(<vscale x 4 x i32> undef, <vscale x 4 x i1> %[[PG]], <vscale x 4 x i32> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u32,_x,)(pg, op);
}

svuint64_t test_svcnt_u64_x(svbool_t pg, svuint64_t op)
{
  // CHECK-LABEL: test_svcnt_u64_x
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2i64(<vscale x 2 x i64> undef, <vscale x 2 x i1> %[[PG]], <vscale x 2 x i64> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_u64,_x,)(pg, op);
}

svuint16_t test_svcnt_f16_x(svbool_t pg, svfloat16_t op)
{
  // CHECK-LABEL: test_svcnt_f16_x
  // CHECK: %[[PG:.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.cnt.nxv8f16(<vscale x 8 x i16> undef, <vscale x 8 x i1> %[[PG]], <vscale x 8 x half> %op)
  // CHECK: ret <vscale x 8 x i16> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f16,_x,)(pg, op);
}

svuint32_t test_svcnt_f32_x(svbool_t pg, svfloat32_t op)
{
  // CHECK-LABEL: test_svcnt_f32_x
  // CHECK: %[[PG:.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.cnt.nxv4f32(<vscale x 4 x i32> undef, <vscale x 4 x i1> %[[PG]], <vscale x 4 x float> %op)
  // CHECK: ret <vscale x 4 x i32> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f32,_x,)(pg, op);
}

svuint64_t test_svcnt_f64_x(svbool_t pg, svfloat64_t op)
{
  // CHECK-LABEL: test_svcnt_f64_x
  // CHECK: %[[PG:.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> %pg)
  // CHECK: %[[INTRINSIC:.*]] = call <vscale x 2 x i64> @llvm.aarch64.sve.cnt.nxv2f64(<vscale x 2 x i64> undef, <vscale x 2 x i1> %[[PG]], <vscale x 2 x double> %op)
  // CHECK: ret <vscale x 2 x i64> %[[INTRINSIC]]
  return SVE_ACLE_FUNC(svcnt,_f64,_x,)(pg, op);
}
