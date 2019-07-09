//==--------- builtins_geometric.cpp - SYCL built-in geometric functions ---==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// This file defines the host versions of functions defined
// in SYCL SPEC section - 4.13.6 Geometric functions.

#include "builtins_helper.hpp"

#include <cmath>

namespace s = cl::sycl;
namespace d = s::detail;

namespace cl {
namespace __host_std {

s::cl_float Dot(s::cl_float2, s::cl_float2);
s::cl_float Dot(s::cl_float3, s::cl_float3);
s::cl_float Dot(s::cl_float4, s::cl_float4);
s::cl_double Dot(s::cl_double2, s::cl_double2);
s::cl_double Dot(s::cl_double3, s::cl_double3);
s::cl_double Dot(s::cl_double4, s::cl_double4);
s::cl_half Dot(s::cl_half2, s::cl_half2);
s::cl_half Dot(s::cl_half3, s::cl_half3);
s::cl_half Dot(s::cl_half4, s::cl_half4);

s::cl_int All(s::cl_int2);
s::cl_int All(s::cl_int3);
s::cl_int All(s::cl_int4);

namespace {

template <typename T> inline T __cross(T p0, T p1) {
  T result(0);
  result.x() = p0.y() * p1.z() - p0.z() * p1.y();
  result.y() = p0.z() * p1.x() - p0.x() * p1.z();
  result.z() = p0.x() * p1.y() - p0.y() * p1.x();
  return result;
}

template <typename T> inline void __FMul_impl(T &r, T p0, T p1) {
  r += p0 * p1;
}

template <typename T> inline T __FMul(T p0, T p1) {
  T result = 0;
  __FMul_impl(result, p0, p1);
  return result;
}

template <typename T>
inline typename std::enable_if<d::is_sgengeo<T>::value, T>::type __length(T t) {
  return std::sqrt(__FMul(t, t));
}

template <typename T>
inline typename std::enable_if<d::is_vgengeo<T>::value,
                               typename T::element_type>::type
__length(T t) {
  return std::sqrt(Dot(t, t));
}

template <typename T>
inline typename std::enable_if<d::is_sgengeo<T>::value, T>::type
__normalize(T t) {
  T r = __length(t);
  return t / T(r);
}

template <typename T>
inline typename std::enable_if<d::is_vgengeo<T>::value, T>::type
__normalize(T t) {
  typename T::element_type r = __length(t);
  return t / T(r);
}

template <typename T>
inline typename std::enable_if<d::is_sgengeo<T>::value, T>::type
__fast_length(T t) {
  return std::sqrt(__FMul(t, t));
}

template <typename T>
inline typename std::enable_if<d::is_vgengeo<T>::value,
                               typename T::element_type>::type
__fast_length(T t) {
  return std::sqrt(Dot(t, t));
}

template <typename T>
inline typename std::enable_if<d::is_vgengeo<T>::value, T>::type
__fast_normalize(T t) {
  if (All(t == T(0.0f)))
    return t;
  typename T::element_type r = std::sqrt(Dot(t, t));
  return t / T(r);
}

} // namespace

// --------------- 4.13.6 Geometric functions. Host implementations ------------
// cross
s::cl_float3 cross(s::cl_float3 p0, s::cl_float3 p1) __NOEXC {
  return __cross(p0, p1);
}
s::cl_float4 cross(s::cl_float4 p0, s::cl_float4 p1) __NOEXC {
  return __cross(p0, p1);
}
s::cl_double3 cross(s::cl_double3 p0, s::cl_double3 p1) __NOEXC {
  return __cross(p0, p1);
}
s::cl_double4 cross(s::cl_double4 p0, s::cl_double4 p1) __NOEXC {
  return __cross(p0, p1);
}
s::cl_half3 cross(s::cl_half3 p0, s::cl_half3 p1) __NOEXC {
  return __cross(p0, p1);
}
s::cl_half4 cross(s::cl_half4 p0, s::cl_half4 p1) __NOEXC {
  return __cross(p0, p1);
}

// FMul
cl_float FMul(s::cl_float p0, s::cl_float p1) { return __FMul(p0, p1); }
cl_double FMul(s::cl_double p0, s::cl_double p1) { return __FMul(p0, p1); }
cl_float FMul(s::cl_half p0, s::cl_half p1) { return __FMul(p0, p1); }

// Dot
MAKE_GEO_1V_2V_RS(Dot, __FMul_impl, s::cl_float, s::cl_float, s::cl_float)
MAKE_GEO_1V_2V_RS(Dot, __FMul_impl, s::cl_double, s::cl_double,
                  s::cl_double)
MAKE_GEO_1V_2V_RS(Dot, __FMul_impl, s::cl_half, s::cl_half, s::cl_half)

// length
cl_float length(s::cl_float p) { return __length(p); }
cl_double length(s::cl_double p) { return __length(p); }
cl_half length(s::cl_half p) { return __length(p); }
cl_float length(s::cl_float2 p) { return __length(p); }
cl_float length(s::cl_float3 p) { return __length(p); }
cl_float length(s::cl_float4 p) { return __length(p); }
cl_double length(s::cl_double2 p) { return __length(p); }
cl_double length(s::cl_double3 p) { return __length(p); }
cl_double length(s::cl_double4 p) { return __length(p); }
cl_half length(s::cl_half2 p) { return __length(p); }
cl_half length(s::cl_half3 p) { return __length(p); }
cl_half length(s::cl_half4 p) { return __length(p); }

// distance
cl_float distance(s::cl_float p0, s::cl_float p1) { return length(p0 - p1); }
cl_float distance(s::cl_float2 p0, s::cl_float2 p1) { return length(p0 - p1); }
cl_float distance(s::cl_float3 p0, s::cl_float3 p1) { return length(p0 - p1); }
cl_float distance(s::cl_float4 p0, s::cl_float4 p1) { return length(p0 - p1); }
cl_double distance(s::cl_double p0, s::cl_double p1) { return length(p0 - p1); }
cl_double distance(s::cl_double2 p0, s::cl_double2 p1) {
  return length(p0 - p1);
}
cl_double distance(s::cl_double3 p0, s::cl_double3 p1) {
  return length(p0 - p1);
}
cl_double distance(s::cl_double4 p0, s::cl_double4 p1) {
  return length(p0 - p1);
}
cl_half distance(s::cl_half p0, s::cl_half p1) { return length(p0 - p1); }
cl_half distance(s::cl_half2 p0, s::cl_half2 p1) { return length(p0 - p1); }
cl_half distance(s::cl_half3 p0, s::cl_half3 p1) { return length(p0 - p1); }
cl_half distance(s::cl_half4 p0, s::cl_half4 p1) { return length(p0 - p1); }

// normalize
s::cl_float normalize(s::cl_float p) { return __normalize(p); }
s::cl_float2 normalize(s::cl_float2 p) { return __normalize(p); }
s::cl_float3 normalize(s::cl_float3 p) { return __normalize(p); }
s::cl_float4 normalize(s::cl_float4 p) { return __normalize(p); }
s::cl_double normalize(s::cl_double p) { return __normalize(p); }
s::cl_double2 normalize(s::cl_double2 p) { return __normalize(p); }
s::cl_double3 normalize(s::cl_double3 p) { return __normalize(p); }
s::cl_double4 normalize(s::cl_double4 p) { return __normalize(p); }
s::cl_half normalize(s::cl_half p) { return __normalize(p); }
s::cl_half2 normalize(s::cl_half2 p) { return __normalize(p); }
s::cl_half3 normalize(s::cl_half3 p) { return __normalize(p); }
s::cl_half4 normalize(s::cl_half4 p) { return __normalize(p); }

// fast_length
cl_float fast_length(s::cl_float p) { return __fast_length(p); }
cl_float fast_length(s::cl_float2 p) { return __fast_length(p); }
cl_float fast_length(s::cl_float3 p) { return __fast_length(p); }
cl_float fast_length(s::cl_float4 p) { return __fast_length(p); }

// fast_normalize
s::cl_float fast_normalize(s::cl_float p) {
  if (p == 0.0f)
    return p;
  s::cl_float r = std::sqrt(FMul(p, p));
  return p / r;
}
s::cl_float2 fast_normalize(s::cl_float2 p) { return __fast_normalize(p); }
s::cl_float3 fast_normalize(s::cl_float3 p) { return __fast_normalize(p); }
s::cl_float4 fast_normalize(s::cl_float4 p) { return __fast_normalize(p); }

// fast_distance
cl_float fast_distance(s::cl_float p0, s::cl_float p1) {
  return fast_length(p0 - p1);
}
cl_float fast_distance(s::cl_float2 p0, s::cl_float2 p1) {
  return fast_length(p0 - p1);
}
cl_float fast_distance(s::cl_float3 p0, s::cl_float3 p1) {
  return fast_length(p0 - p1);
}
cl_float fast_distance(s::cl_float4 p0, s::cl_float4 p1) {
  return fast_length(p0 - p1);
}

} // namespace __host_std
} // namespace cl
