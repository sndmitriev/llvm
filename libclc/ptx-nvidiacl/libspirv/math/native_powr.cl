//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <spirv/spirv.h>

#include "../../include/libdevice.h"
#include <clcmacro.h>

#define __CLC_FUNCTION __spirv_ocl_native_powr
#define __CLC_BUILTIN __nv_fast_pow
#define __CLC_BUILTIN_F __CLC_XCONCAT(__CLC_BUILTIN, f)
#define __FLOAT_ONLY
#include <math/binary_builtin.inc>
