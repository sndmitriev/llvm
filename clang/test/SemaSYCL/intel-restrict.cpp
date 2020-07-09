// RUN: %clang_cc1 %s -fsyntax-only -fsycl -fsycl-is-device -triple spir64 -DCHECKDIAG -verify
// RUN: %clang_cc1 %s -fsyntax-only -ast-dump -fsycl -fsycl-is-device -triple spir64 | FileCheck %s

[[intel::kernel_args_restrict]] // expected-warning{{'kernel_args_restrict' attribute ignored}}
void func_ignore() {}

struct FuncObj {
  [[intel::kernel_args_restrict]]
  void operator()() {}
};

template <typename name, typename Func>
__attribute__((sycl_kernel)) void kernel(Func kernelFunc) {
  kernelFunc();
#ifdef CHECKDIAG
  [[intel::kernel_args_restrict]] int invalid = 42; // expected-error{{'kernel_args_restrict' attribute only applies to functions}}
#endif
}

int main() {
  // CHECK-LABEL: FunctionDecl {{.*}}test_kernel1
  // CHECK:       SYCLIntelKernelArgsRestrictAttr
  kernel<class test_kernel1>(
      FuncObj());

  // CHECK-LABEL: FunctionDecl {{.*}}test_kernel2
  // CHECK:       SYCLIntelKernelArgsRestrictAttr
  kernel<class test_kernel2>(
      []() [[intel::kernel_args_restrict]] {});

  // CHECK-LABEL: FunctionDecl {{.*}}test_kernel3
  // CHECK-NOT:   SYCLIntelKernelArgsRestrictAttr
  kernel<class test_kernel3>(
      []() {func_ignore();});
}
