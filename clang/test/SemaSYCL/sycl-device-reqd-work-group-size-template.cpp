// RUN: %clang_cc1 -fsycl -fsycl-is-device -fsyntax-only -ast-dump -verify -pedantic %s | FileCheck %s

// Test that checks template parameter support for 'cl::reqd_work_group_size' attribute on sycl device.

// Test that checks wrong function template instantiation and ensures that the type
// is checked properly when instantiating from the template definition.

template <typename Ty, typename Ty1, typename Ty2>
// expected-error@+1{{'reqd_work_group_size' attribute requires an integer constant}}
[[cl::reqd_work_group_size(Ty{}, Ty1{}, Ty2{})]] void func() {}

struct S {};
void var() {
  //expected-note@+1{{in instantiation of function template specialization 'func<S, S, S>' requested here}}
  func<S, S, S>();
}

// Test that checks expression is not a constant expression.
int foo();
// expected-error@+1{{'reqd_work_group_size' attribute requires an integer constant}}
[[cl::reqd_work_group_size(foo() + 12, foo() + 12, foo() + 12)]] void func1();

// Test that checks expression is a constant expression.
constexpr int bar() { return 0; }
[[cl::reqd_work_group_size(bar() + 12, bar() + 12, bar() + 12)]] void func2(); // OK

// Test that checks template parameter suppport on member function of class template.
template <int SIZE, int SIZE1, int SIZE2>
class KernelFunctor {
public:
  [[cl::reqd_work_group_size(SIZE, SIZE1, SIZE2)]] void operator()() {}
};

int main() {
  KernelFunctor<16, 1, 1>();
}

// CHECK: ClassTemplateDecl {{.*}} {{.*}} KernelFunctor
// CHECK: ClassTemplateSpecializationDecl {{.*}} {{.*}} class KernelFunctor definition
// CHECK: CXXRecordDecl {{.*}} {{.*}} implicit class KernelFunctor
// CHECK: ReqdWorkGroupSizeAttr {{.*}}
// CHECK: SubstNonTypeTemplateParmExpr {{.*}}
// CHECK-NEXT: NonTypeTemplateParmDecl {{.*}}
// CHECK-NEXT: IntegerLiteral{{.*}}16{{$}}
// CHECK: SubstNonTypeTemplateParmExpr {{.*}}
// CHECK-NEXT: NonTypeTemplateParmDecl {{.*}}
// CHECK-NEXT: IntegerLiteral{{.*}}1{{$}}
// CHECK: SubstNonTypeTemplateParmExpr {{.*}}
// CHECK-NEXT: NonTypeTemplateParmDecl {{.*}}
// CHECK-NEXT: IntegerLiteral{{.*}}1{{$}}

// Test that checks template parameter suppport on function.
template <int N, int N1, int N2>
[[cl::reqd_work_group_size(N, N1, N2)]] void func3() {}

int check() {
  func3<8, 8, 8>();
  return 0;
}

// CHECK: FunctionTemplateDecl {{.*}} {{.*}} func3
// CHECK: NonTypeTemplateParmDecl {{.*}} {{.*}} referenced 'int' depth 0 index 0 N
// CHECK: NonTypeTemplateParmDecl {{.*}} {{.*}} referenced 'int' depth 0 index 1 N1
// CHECK: NonTypeTemplateParmDecl {{.*}} {{.*}} referenced 'int' depth 0 index 2 N2
// CHECK: FunctionDecl {{.*}} {{.*}} func3 'void ()'
// CHECK: ReqdWorkGroupSizeAttr {{.*}}
// CHECK: SubstNonTypeTemplateParmExpr {{.*}}
// CHECK-NEXT: NonTypeTemplateParmDecl {{.*}}
// CHECK-NEXT: IntegerLiteral{{.*}}8{{$}}
// CHECK: SubstNonTypeTemplateParmExpr {{.*}}
// CHECK-NEXT: NonTypeTemplateParmDecl {{.*}}
// CHECK-NEXT: IntegerLiteral{{.*}}8{{$}}
// CHECK: SubstNonTypeTemplateParmExpr {{.*}}
// CHECK-NEXT: NonTypeTemplateParmDecl {{.*}}
// CHECK-NEXT: IntegerLiteral{{.*}}8{{$}}
