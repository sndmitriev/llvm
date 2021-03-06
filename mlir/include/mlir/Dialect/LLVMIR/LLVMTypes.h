//===- LLVMDialect.h - MLIR LLVM dialect types ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the types for the LLVM dialect in MLIR. These MLIR types
// correspond to the LLVM IR type system.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_LLVMIR_LLVMTYPES_H_
#define MLIR_DIALECT_LLVMIR_LLVMTYPES_H_

#include "mlir/IR/Types.h"

namespace llvm {
class ElementCount;
class TypeSize;
} // namespace llvm

namespace mlir {

class DialectAsmParser;
class DialectAsmPrinter;

namespace LLVM {
class LLVMDialect;

namespace detail {
struct LLVMFunctionTypeStorage;
struct LLVMIntegerTypeStorage;
struct LLVMPointerTypeStorage;
struct LLVMStructTypeStorage;
struct LLVMTypeAndSizeStorage;
} // namespace detail

class LLVMBFloatType;
class LLVMHalfType;
class LLVMFloatType;
class LLVMDoubleType;
class LLVMFP128Type;
class LLVMX86FP80Type;
class LLVMIntegerType;

//===----------------------------------------------------------------------===//
// LLVMType.
//===----------------------------------------------------------------------===//

/// Base class for LLVM dialect types.
///
/// The LLVM dialect in MLIR fully reflects the LLVM IR type system, prodiving a
/// separate MLIR type for each LLVM IR type. All types are represented as
/// separate subclasses and are compatible with the isa/cast infrastructure.
///
/// The LLVM dialect type system is closed: parametric types can only refer to
/// other LLVM dialect types. This is consistent with LLVM IR and enables a more
/// concise pretty-printing format.
///
/// Similarly to other MLIR types, LLVM dialect types are owned by the MLIR
/// context, have an immutable identifier (for most types except identified
/// structs, the entire type is the identifier) and are thread-safe.
///
/// This class is a thin common base class for different types available in the
/// LLVM dialect. It intentionally does not provide the API similar to
/// llvm::Type to avoid confusion and highlight potentially expensive operations
/// (e.g., type creation in MLIR takes a lock, so it's better to cache types).
class LLVMType : public Type {
public:
  /// Inherit base constructors.
  using Type::Type;

  /// Support for PointerLikeTypeTraits.
  using Type::getAsOpaquePointer;
  static LLVMType getFromOpaquePointer(const void *ptr) {
    return LLVMType(static_cast<ImplType *>(const_cast<void *>(ptr)));
  }

  /// Support for isa/cast.
  static bool classof(Type type);

  LLVMDialect &getDialect();
};

//===----------------------------------------------------------------------===//
// Trivial types.
//===----------------------------------------------------------------------===//

// Batch-define trivial types.
#define DEFINE_TRIVIAL_LLVM_TYPE(ClassName)                                    \
  class ClassName : public Type::TypeBase<ClassName, LLVMType, TypeStorage> {  \
  public:                                                                      \
    using Base::Base;                                                          \
  }

DEFINE_TRIVIAL_LLVM_TYPE(LLVMVoidType);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMHalfType);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMBFloatType);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMFloatType);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMDoubleType);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMFP128Type);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMX86FP80Type);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMPPCFP128Type);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMX86MMXType);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMTokenType);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMLabelType);
DEFINE_TRIVIAL_LLVM_TYPE(LLVMMetadataType);

#undef DEFINE_TRIVIAL_LLVM_TYPE

//===----------------------------------------------------------------------===//
// LLVMArrayType.
//===----------------------------------------------------------------------===//

/// LLVM dialect array type. It is an aggregate type representing consecutive
/// elements in memory, parameterized by the number of elements and the element
/// type.
class LLVMArrayType : public Type::TypeBase<LLVMArrayType, LLVMType,
                                            detail::LLVMTypeAndSizeStorage> {
public:
  /// Inherit base constructors.
  using Base::Base;

  /// Checks if the given type can be used inside an array type.
  static bool isValidElementType(LLVMType type);

  /// Gets or creates an instance of LLVM dialect array type containing
  /// `numElements` of `elementType`, in the same context as `elementType`.
  static LLVMArrayType get(LLVMType elementType, unsigned numElements);
  static LLVMArrayType getChecked(Location loc, LLVMType elementType,
                                  unsigned numElements);

  /// Returns the element type of the array.
  LLVMType getElementType();

  /// Returns the number of elements in the array type.
  unsigned getNumElements();

  /// Verifies that the type about to be constructed is well-formed.
  static LogicalResult verifyConstructionInvariants(Location loc,
                                                    LLVMType elementType,
                                                    unsigned numElements);
};

//===----------------------------------------------------------------------===//
// LLVMFunctionType.
//===----------------------------------------------------------------------===//

/// LLVM dialect function type. It consists of a single return type (unlike MLIR
/// which can have multiple), a list of parameter types and can optionally be
/// variadic.
class LLVMFunctionType
    : public Type::TypeBase<LLVMFunctionType, LLVMType,
                            detail::LLVMFunctionTypeStorage> {
public:
  /// Inherit base constructors.
  using Base::Base;

  /// Checks if the given type can be used an argument in a function type.
  static bool isValidArgumentType(LLVMType type);

  /// Checks if the given type can be used as a result in a function type.
  static bool isValidResultType(LLVMType type);

  /// Returns whether the function is variadic.
  bool isVarArg();

  /// Gets or creates an instance of LLVM dialect function in the same context
  /// as the `result` type.
  static LLVMFunctionType get(LLVMType result, ArrayRef<LLVMType> arguments,
                              bool isVarArg = false);
  static LLVMFunctionType getChecked(Location loc, LLVMType result,
                                     ArrayRef<LLVMType> arguments,
                                     bool isVarArg = false);

  /// Returns the result type of the function.
  LLVMType getReturnType();

  /// Returns the number of arguments to the function.
  unsigned getNumParams();

  /// Returns `i`-th argument of the function. Asserts on out-of-bounds.
  LLVMType getParamType(unsigned i);

  /// Returns a list of argument types of the function.
  ArrayRef<LLVMType> getParams();
  ArrayRef<LLVMType> params() { return getParams(); }

  /// Verifies that the type about to be constructed is well-formed.
  static LogicalResult
  verifyConstructionInvariants(Location loc, LLVMType result,
                               ArrayRef<LLVMType> arguments, bool);
};

//===----------------------------------------------------------------------===//
// LLVMIntegerType.
//===----------------------------------------------------------------------===//

/// LLVM dialect signless integer type parameterized by bitwidth.
class LLVMIntegerType : public Type::TypeBase<LLVMIntegerType, LLVMType,
                                              detail::LLVMIntegerTypeStorage> {
public:
  /// Inherit base constructor.
  using Base::Base;

  /// Gets or creates an instance of the integer of the specified `bitwidth` in
  /// the given context.
  static LLVMIntegerType get(MLIRContext *ctx, unsigned bitwidth);
  static LLVMIntegerType getChecked(Location loc, unsigned bitwidth);

  /// Returns the bitwidth of this integer type.
  unsigned getBitWidth();

  /// Verifies that the type about to be constructed is well-formed.
  static LogicalResult verifyConstructionInvariants(Location loc,
                                                    unsigned bitwidth);
};

//===----------------------------------------------------------------------===//
// LLVMPointerType.
//===----------------------------------------------------------------------===//

/// LLVM dialect pointer type. This type typically represents a reference to an
/// object in memory. It is parameterized by the element type and the address
/// space.
class LLVMPointerType : public Type::TypeBase<LLVMPointerType, LLVMType,
                                              detail::LLVMPointerTypeStorage> {
public:
  /// Inherit base constructors.
  using Base::Base;

  /// Checks if the given type can have a pointer type pointing to it.
  static bool isValidElementType(LLVMType type);

  /// Gets or creates an instance of LLVM dialect pointer type pointing to an
  /// object of `pointee` type in the given address space. The pointer type is
  /// created in the same context as `pointee`.
  static LLVMPointerType get(LLVMType pointee, unsigned addressSpace = 0);
  static LLVMPointerType getChecked(Location loc, LLVMType pointee,
                                    unsigned addressSpace = 0);

  /// Returns the pointed-to type.
  LLVMType getElementType();

  /// Returns the address space of the pointer.
  unsigned getAddressSpace();

  /// Verifies that the type about to be constructed is well-formed.
  static LogicalResult verifyConstructionInvariants(Location loc,
                                                    LLVMType pointee, unsigned);
};

//===----------------------------------------------------------------------===//
// LLVMStructType.
//===----------------------------------------------------------------------===//

/// LLVM dialect structure type representing a collection of different-typed
/// elements manipulated together. Structured can optionally be packed, meaning
/// that their elements immediately follow each other in memory without
/// accounting for potential alignment.
///
/// Structure types can be identified (named) or literal. Literal structures
/// are uniquely represented by the list of types they contain and packedness.
/// Literal structure types are immutable after construction.
///
/// Identified structures are uniquely represented by their name, a string. They
/// have a mutable component, consisting of the list of types they contain,
/// the packedness and the opacity bits. Identified structs can be created
/// without providing the lists of element types, making them suitable to
/// represent recursive, i.e. self-referring, structures. Identified structs
/// without body are considered opaque. For such structs, one can set the body.
/// Identified structs can be created as intentionally-opaque, implying that the
/// caller does not intend to ever set the body (e.g. forward-declarations of
/// structs from another module) and wants to disallow further modification of
/// the body. For intentionally-opaque structs or non-opaque structs with the
/// body, one is not allowed to set another body (however, one can set exactly
/// the same body).
///
/// Note that the packedness of the struct takes place in uniquing of literal
/// structs, but does not in uniquing of identified structs.
class LLVMStructType : public Type::TypeBase<LLVMStructType, LLVMType,
                                             detail::LLVMStructTypeStorage> {
public:
  /// Inherit base constructors.
  using Base::Base;

  /// Checks if the given type can be contained in a structure type.
  static bool isValidElementType(LLVMType type);

  /// Gets or creates an identified struct with the given name in the provided
  /// context. Note that unlike llvm::StructType::create, this function will
  /// _NOT_ rename a struct in case a struct with the same name already exists
  /// in the context. Instead, it will just return the existing struct,
  /// similarly to the rest of MLIR type ::get methods.
  static LLVMStructType getIdentified(MLIRContext *context, StringRef name);
  static LLVMStructType getIdentifiedChecked(Location loc, StringRef name);

  /// Gets a new identified struct with the given body. The body _cannot_ be
  /// changed later. If a struct with the given name already exists, renames
  /// the struct by appending a `.` followed by a number to the name. Renaming
  /// happens even if the existing struct has the same body.
  static LLVMStructType getNewIdentified(MLIRContext *context, StringRef name,
                                         ArrayRef<LLVMType> elements,
                                         bool isPacked = false);

  /// Gets or creates a literal struct with the given body in the provided
  /// context.
  static LLVMStructType getLiteral(MLIRContext *context,
                                   ArrayRef<LLVMType> types,
                                   bool isPacked = false);
  static LLVMStructType getLiteralChecked(Location loc,
                                          ArrayRef<LLVMType> types,
                                          bool isPacked = false);

  /// Gets or creates an intentionally-opaque identified struct. Such a struct
  /// cannot have its body set. To create an opaque struct with a mutable body,
  /// use `getIdentified`. Note that unlike llvm::StructType::create, this
  /// function will _NOT_ rename a struct in case a struct with the same name
  /// already exists in the context. Instead, it will just return the existing
  /// struct, similarly to the rest of MLIR type ::get methods.
  static LLVMStructType getOpaque(StringRef name, MLIRContext *context);
  static LLVMStructType getOpaqueChecked(Location loc, StringRef name);

  /// Set the body of an identified struct. Returns failure if the body could
  /// not be set, e.g. if the struct already has a body or if it was marked as
  /// intentionally opaque. This might happen in a multi-threaded context when a
  /// different thread modified the struct after it was created. Most callers
  /// are likely to assert this always succeeds, but it is possible to implement
  /// a local renaming scheme based on the result of this call.
  LogicalResult setBody(ArrayRef<LLVMType> types, bool isPacked);

  /// Checks if a struct is packed.
  bool isPacked();

  /// Checks if a struct is identified.
  bool isIdentified();

  /// Checks if a struct is opaque.
  bool isOpaque();

  /// Checks if a struct is initialized.
  bool isInitialized();

  /// Returns the name of an identified struct.
  StringRef getName();

  /// Returns the list of element types contained in a non-opaque struct.
  ArrayRef<LLVMType> getBody();

  /// Verifies that the type about to be constructed is well-formed.
  static LogicalResult verifyConstructionInvariants(Location, StringRef, bool);
  static LogicalResult
  verifyConstructionInvariants(Location loc, ArrayRef<LLVMType> types, bool);
};

//===----------------------------------------------------------------------===//
// LLVMVectorType.
//===----------------------------------------------------------------------===//

/// LLVM dialect vector type, represents a sequence of elements that can be
/// processed as one, typically in SIMD context. This is a base class for fixed
/// and scalable vectors.
class LLVMVectorType : public LLVMType {
public:
  /// Inherit base constructor.
  using LLVMType::LLVMType;

  /// Support type casting functionality.
  static bool classof(Type type);

  /// Checks if the given type can be used in a vector type.
  static bool isValidElementType(LLVMType type);

  /// Returns the element type of the vector.
  LLVMType getElementType();

  /// Returns the number of elements in the vector.
  llvm::ElementCount getElementCount();

  /// Verifies that the type about to be constructed is well-formed.
  static LogicalResult verifyConstructionInvariants(Location loc,
                                                    LLVMType elementType,
                                                    unsigned numElements);
};

//===----------------------------------------------------------------------===//
// LLVMFixedVectorType.
//===----------------------------------------------------------------------===//

/// LLVM dialect fixed vector type, represents a sequence of elements of known
/// length that can be processed as one.
class LLVMFixedVectorType
    : public Type::TypeBase<LLVMFixedVectorType, LLVMVectorType,
                            detail::LLVMTypeAndSizeStorage> {
public:
  /// Inherit base constructor.
  using Base::Base;
  using LLVMVectorType::verifyConstructionInvariants;

  /// Gets or creates a fixed vector type containing `numElements` of
  /// `elementType` in the same context as `elementType`.
  static LLVMFixedVectorType get(LLVMType elementType, unsigned numElements);
  static LLVMFixedVectorType getChecked(Location loc, LLVMType elementType,
                                        unsigned numElements);

  /// Returns the number of elements in the fixed vector.
  unsigned getNumElements();
};

//===----------------------------------------------------------------------===//
// LLVMScalableVectorType.
//===----------------------------------------------------------------------===//

/// LLVM dialect scalable vector type, represents a sequence of elements of
/// unknown length that is known to be divisible by some constant. These
/// elements can be processed as one in SIMD context.
class LLVMScalableVectorType
    : public Type::TypeBase<LLVMScalableVectorType, LLVMVectorType,
                            detail::LLVMTypeAndSizeStorage> {
public:
  /// Inherit base constructor.
  using Base::Base;
  using LLVMVectorType::verifyConstructionInvariants;

  /// Gets or creates a scalable vector type containing a non-zero multiple of
  /// `minNumElements` of `elementType` in the same context as `elementType`.
  static LLVMScalableVectorType get(LLVMType elementType,
                                    unsigned minNumElements);
  static LLVMScalableVectorType getChecked(Location loc, LLVMType elementType,
                                           unsigned minNumElements);

  /// Returns the scaling factor of the number of elements in the vector. The
  /// vector contains at least the resulting number of elements, or any non-zero
  /// multiple of this number.
  unsigned getMinNumElements();
};

//===----------------------------------------------------------------------===//
// Printing and parsing.
//===----------------------------------------------------------------------===//

namespace detail {
/// Parses an LLVM dialect type.
LLVMType parseType(DialectAsmParser &parser);

/// Prints an LLVM Dialect type.
void printType(LLVMType type, DialectAsmPrinter &printer);
} // namespace detail

//===----------------------------------------------------------------------===//
// Utility functions.
//===----------------------------------------------------------------------===//

/// Returns `true` if the given type is compatible with the LLVM dialect.
inline bool isCompatibleType(Type type) { return type.isa<LLVMType>(); }

inline bool isCompatibleFloatingPointType(Type type) {
  return type.isa<LLVMHalfType, LLVMBFloatType, LLVMFloatType, LLVMDoubleType,
                  LLVMFP128Type, LLVMX86FP80Type>();
}

/// Returns the size of the given primitive LLVM dialect-compatible type
/// (including vectors) in bits, for example, the size of !llvm.i16 is 16 and
/// the size of !llvm.vec<4 x i16> is 64. Returns 0 for non-primitive
/// (aggregates such as struct) or types that don't have a size (such as void).
llvm::TypeSize getPrimitiveTypeSizeInBits(Type type);

} // namespace LLVM
} // namespace mlir

//===----------------------------------------------------------------------===//
// Support for hashing and containers.
//===----------------------------------------------------------------------===//

namespace llvm {

// LLVMType instances hash just like pointers.
template <>
struct DenseMapInfo<mlir::LLVM::LLVMType> {
  static mlir::LLVM::LLVMType getEmptyKey() {
    void *pointer = llvm::DenseMapInfo<void *>::getEmptyKey();
    return mlir::LLVM::LLVMType(
        static_cast<mlir::LLVM::LLVMType::ImplType *>(pointer));
  }
  static mlir::LLVM::LLVMType getTombstoneKey() {
    void *pointer = llvm::DenseMapInfo<void *>::getTombstoneKey();
    return mlir::LLVM::LLVMType(
        static_cast<mlir::LLVM::LLVMType::ImplType *>(pointer));
  }
  static unsigned getHashValue(mlir::LLVM::LLVMType val) {
    return mlir::hash_value(val);
  }
  static bool isEqual(mlir::LLVM::LLVMType lhs, mlir::LLVM::LLVMType rhs) {
    return lhs == rhs;
  }
};

// LLVMType behaves like a pointer similarly to mlir::Type.
template <>
struct PointerLikeTypeTraits<mlir::LLVM::LLVMType> {
  static inline void *getAsVoidPointer(mlir::LLVM::LLVMType type) {
    return const_cast<void *>(type.getAsOpaquePointer());
  }
  static inline mlir::LLVM::LLVMType getFromVoidPointer(void *ptr) {
    return mlir::LLVM::LLVMType::getFromOpaquePointer(ptr);
  }
  static constexpr int NumLowBitsAvailable =
      PointerLikeTypeTraits<mlir::Type>::NumLowBitsAvailable;
};

} // namespace llvm

#endif // MLIR_DIALECT_LLVMIR_LLVMTYPES_H_
