
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Autogenerated by gen-libclc-test.py

// RUN: %clang -emit-llvm -S -o - %s | FileCheck %s

#include <spirv/spirv_types.h>

// CHECK-NOT: declare {{.*}} @_Z
// CHECK-NOT: call {{[^ ]*}} bitcast
#ifdef cl_khr_fp16
__attribute__((overloadable)) __clc_vec4_fp16_t
test___spirv_ConvertSToF_Rhalf4_rte(__clc_vec4_int8_t args_0) {
  return __spirv_ConvertSToF_Rhalf4_rte(args_0);
}

#endif
#ifdef cl_khr_fp16
__attribute__((overloadable)) __clc_vec4_fp16_t
test___spirv_ConvertSToF_Rhalf4_rte(__clc_vec4_int16_t args_0) {
  return __spirv_ConvertSToF_Rhalf4_rte(args_0);
}

#endif
#ifdef cl_khr_fp16
__attribute__((overloadable)) __clc_vec4_fp16_t
test___spirv_ConvertSToF_Rhalf4_rte(__clc_vec4_int32_t args_0) {
  return __spirv_ConvertSToF_Rhalf4_rte(args_0);
}

#endif
#ifdef cl_khr_fp16
__attribute__((overloadable)) __clc_vec4_fp16_t
test___spirv_ConvertSToF_Rhalf4_rte(__clc_vec4_int64_t args_0) {
  return __spirv_ConvertSToF_Rhalf4_rte(args_0);
}

#endif
