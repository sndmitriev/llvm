// RUN: %clang_cc1 -fsycl -fsycl-is-device -fsyntax-only -Wno-sycl-2017-compat -verify -pedantic %s

// Test for Intel FPGA loop attributes applied not to a loop
void foo() {
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::ivdep]] int a[10];
  // expected-error@+1 {{ loop attributes must be applied to for, while, or do statements}}
  [[intel::ivdep(2)]] int b[10];
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::ii(2)]] int c[10];
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::max_concurrency(2)]] int d[10];

  int arr[10];
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::ivdep(arr)]] int e[10];
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::ivdep(arr, 2)]] int f[10];

  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::disable_loop_pipelining]] int g[10];
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::loop_coalesce(2)]] int h[10];
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::max_interleaving(4)]] int i[10];
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::speculated_iterations(6)]] int j[10];
  // expected-error@+1 {{intelfpga loop attributes must be applied to for, while, or do statements}}
  [[intel::nofusion]] int k[10];
}

// Test for deprecated spelling of Intel FPGA loop attributes
void foo_deprecated() {
  int a[10];
  // expected-warning@+2 {{attribute 'intelfpga::ivdep' is deprecated}}
  // expected-note@+1 {{did you mean to use 'intel::ivdep' instead?}}
  [[intelfpga::ivdep(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-warning@+2 {{attribute 'intelfpga::ii' is deprecated}}
  // expected-note@+1 {{did you mean to use 'intel::ii' instead?}}
  [[intelfpga::ii(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-warning@+2 {{attribute 'intelfpga::max_concurrency' is deprecated}}
  // expected-note@+1 {{did you mean to use 'intel::max_concurrency' instead?}}
  [[intelfpga::max_concurrency(4)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-warning@+2 {{attribute 'intelfpga::max_interleaving' is deprecated}}
  // expected-note@+1 {{did you mean to use 'intel::max_interleaving' instead?}}
  [[intelfpga::max_interleaving(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-warning@+2 {{attribute 'intelfpga::disable_loop_pipelining' is deprecated}}
  // expected-note@+1 {{did you mean to use 'intel::disable_loop_pipelining' instead?}}
  [[intelfpga::disable_loop_pipelining]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-warning@+2 {{attribute 'intelfpga::loop_coalesce' is deprecated}}
  // expected-note@+1 {{did you mean to use 'intel::loop_coalesce' instead?}}
  [[intelfpga::loop_coalesce(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-warning@+2 {{attribute 'intelfpga::speculated_iterations' is deprecated}}
  // expected-note@+1 {{did you mean to use 'intel::speculated_iterations' instead?}}
  [[intelfpga::speculated_iterations(6)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
}

// Test for incorrect number of arguments for Intel FPGA loop attributes
void boo() {
  int a[10];
  int b[10];
  // expected-error@+1 {{duplicate argument to 'ivdep'; attribute requires one or both of a safelen and array}}
  [[intel::ivdep(2, 2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'ii' attribute takes at least 1 argument; attribute ignored}}
  [[intel::ii]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'ii' attribute takes no more than 1 argument; attribute ignored}}
  [[intel::ii(2, 2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'max_concurrency' attribute takes at least 1 argument; attribute ignored}}
  [[intel::max_concurrency]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'max_concurrency' attribute takes no more than 1 argument; attribute ignored}}
  [[intel::max_concurrency(2, 2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-error@+1 {{duplicate argument to 'ivdep'; attribute requires one or both of a safelen and array}}
  [[intel::ivdep(2, 3)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{duplicate argument to 'ivdep'; attribute requires one or both of a safelen and array}}
  [[intel::ivdep(a, b)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{unknown argument to 'ivdep'; expected integer or array variable}}
  [[intel::ivdep(2, 3.0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-warning@+1 {{'disable_loop_pipelining' attribute takes no more than 0 arguments; attribute ignored}}
  [[intel::disable_loop_pipelining(0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'loop_coalesce' attribute takes no more than 1 argument; attribute ignored}}
  [[intel::loop_coalesce(2, 3)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'max_interleaving' attribute takes at least 1 argument; attribute ignored}}
  [[intel::max_interleaving]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'max_interleaving' attribute takes no more than 1 argument; attribute ignored}}
  [[intel::max_interleaving(2, 4)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'speculated_iterations' attribute takes at least 1 argument; attribute ignored}}
  [[intel::speculated_iterations]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'speculated_iterations' attribute takes no more than 1 argument; attribute ignored}}
  [[intel::speculated_iterations(1, 2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-warning@+1 {{'nofusion' attribute takes no more than 0 arguments; attribute ignored}}
  [[intel::nofusion(0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
}

// Test for incorrect argument value for Intel FPGA loop attributes
void goo() {
  int a[10];
  // no diagnostics are expected
  [[intel::disable_loop_pipelining]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // no diagnostics are expected
  [[intel::max_concurrency(0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'ivdep' attribute requires a positive integral compile time constant expression}}
  [[intel::ivdep(0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'ii' attribute requires a positive integral compile time constant expression}}
  [[intel::ii(0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'max_concurrency' attribute requires a non-negative integral compile time constant expression}}
  [[intel::max_concurrency(-1)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'loop_coalesce' attribute requires a positive integral compile time constant expression}}
  [[intel::loop_coalesce(0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'max_interleaving' attribute requires a non-negative integral compile time constant expression}}
  [[intel::max_interleaving(-1)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'speculated_iterations' attribute requires a non-negative integral compile time constant expression}}
  [[intel::speculated_iterations(-1)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{unknown argument to 'ivdep'; expected integer or array variable}}
  [[intel::ivdep("test123")]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'ii' attribute requires an integer constant}}
  [[intel::ii("test123")]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'max_concurrency' attribute requires an integer constant}}
  [[intel::max_concurrency("test123")]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'loop_coalesce' attribute requires an integer constant}}
  [[intel::loop_coalesce("test123")]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'max_interleaving' attribute requires an integer constant}}
  [[intel::max_interleaving("test123")]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{'speculated_iterations' attribute requires an integer constant}}
  [[intel::speculated_iterations("test123")]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{unknown argument to 'ivdep'; expected integer or array variable}}
  [[intel::ivdep("test123")]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // no diagnostics are expected
  [[intel::ivdep(a, 2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // no diagnostics are expected
  [[intel::ivdep(2, a)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  int *ptr;
  // no diagnostics are expected
  [[intel::ivdep(2, ptr)]] for (int i = 0; i != 10; ++i)
      ptr[i] = 0;

  struct S {
    int arr[10];
    int *ptr;
  } s;

  // no diagnostics are expected
  [[intel::ivdep(2, s.arr)]] for (int i = 0; i != 10; ++i)
      s.arr[i] = 0;
  // no diagnostics are expected
  [[intel::ivdep(2, s.ptr)]] for (int i = 0; i != 10; ++i)
      s.ptr[i] = 0;

  // no diagnostics are expected
  [[intel::nofusion]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
}

// Test for Intel FPGA loop attributes duplication
void zoo() {
  int a[10];
  // no diagnostics are expected
  [[intel::ivdep]]
  [[intel::max_concurrency(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::ivdep]]
  // expected-warning@+2 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen INF >= safelen INF}}
  // expected-note@-2 {{previous attribute is here}}
  [[intel::ivdep]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::ivdep]]
  // expected-warning@+2 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen INF >= safelen 2}}
  // expected-note@-2 {{previous attribute is here}}
  [[intel::ivdep(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::ivdep(2)]]
  // expected-warning@-1 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen 4 >= safelen 2}}
  // expected-note@+1 {{previous attribute is here}}
  [[intel::ivdep(4)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::max_concurrency(2)]]
  // expected-error@-1 {{duplicate Intel FPGA loop attribute 'max_concurrency'}}
  [[intel::max_concurrency(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::ii(2)]]
  // expected-error@-1 {{duplicate Intel FPGA loop attribute 'ii'}}
  [[intel::ii(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::ii(2)]]
  // expected-error@-1 {{duplicate Intel FPGA loop attribute 'ii'}}
  [[intel::max_concurrency(2)]]
  [[intel::ii(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::disable_loop_pipelining]]
  // expected-error@-1 {{duplicate Intel FPGA loop attribute 'disable_loop_pipelining'}}
  [[intel::disable_loop_pipelining]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::loop_coalesce(2)]]
  // expected-error@-1 {{duplicate Intel FPGA loop attribute 'loop_coalesce'}}
  [[intel::max_interleaving(1)]]
  [[intel::loop_coalesce]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::max_interleaving(1)]]
  // expected-error@-1 {{duplicate Intel FPGA loop attribute 'max_interleaving'}}
  [[intel::speculated_iterations(1)]]
  [[intel::max_interleaving(4)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::speculated_iterations(1)]]
  // expected-error@-1 {{duplicate Intel FPGA loop attribute 'speculated_iterations'}}
  [[intel::loop_coalesce]]
  [[intel::speculated_iterations(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  [[intel::ivdep]]
  // expected-warning@+2 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen INF >= safelen INF}}
  // expected-note@-2 {{previous attribute is here}}
  [[intel::ivdep]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::ivdep(2)]]
  // expected-warning@-1 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen INF >= safelen 2}}
  // expected-note@+1 {{previous attribute is here}}
  [[intel::ivdep]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::ivdep(a, 2)]]
  // expected-warning@-1 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen INF >= safelen 2}}
  // expected-note@+1 {{previous attribute is here}}
  [[intel::ivdep(a)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  [[intel::ivdep(2)]]
  // expected-warning@-1 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen 4 >= safelen 2}}
  // expected-note@+1 {{previous attribute is here}}
  [[intel::ivdep(4)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // no diagnostics are expected
  [[intel::ivdep(a)]]
  [[intel::ivdep(2)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  [[intel::ivdep(a, 2)]]
  // expected-warning@-1 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen INF >= safelen 2}}
  // expected-note@+1 {{previous attribute is here}}
  [[intel::ivdep]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // Ensure we only diagnose conflict with the 'worst', not all.
  // expected-warning@+1 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen 5 >= safelen 3}}
  [[intel::ivdep(3)]]
  // expected-warning@+1 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen 5 >= safelen 4}}
  [[intel::ivdep(4)]]
  // expected-note@+1 2 {{previous attribute is here}}
  [[intel::ivdep(5)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  [[intel::ivdep(a, 2)]]
  // expected-warning@-1 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen 3 >= safelen 2}}
  // expected-note@+1 {{previous attribute is here}}
  [[intel::ivdep(a, 3)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  [[intel::nofusion]]
  // expected-error@-1 {{duplicate Intel FPGA loop attribute 'nofusion'}}
  [[intel::nofusion]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
}

// Test for Intel FPGA loop attributes compatibility
void loop_attrs_compatibility() {
  int a[10];
  // no diagnostics are expected
  [[intel::disable_loop_pipelining]]
  [[intel::loop_coalesce]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{disable_loop_pipelining and max_interleaving attributes are not compatible}}
  [[intel::disable_loop_pipelining]]
  [[intel::max_interleaving(0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{disable_loop_pipelining and speculated_iterations attributes are not compatible}}
  [[intel::speculated_iterations(0)]]
  [[intel::disable_loop_pipelining]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{disable_loop_pipelining and max_concurrency attributes are not compatible}}
  [[intel::disable_loop_pipelining]]
  [[intel::max_concurrency(0)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{disable_loop_pipelining and ii attributes are not compatible}}
  [[intel::ii(10)]]
  [[intel::disable_loop_pipelining]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // expected-error@+1 {{disable_loop_pipelining and ivdep attributes are not compatible}}
  [[intel::disable_loop_pipelining]]
  [[intel::ivdep]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
  // no diagnostics are expected
  [[intel::disable_loop_pipelining]]
  [[intel::nofusion]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
}

template<int A, int B, int C>
void ivdep_dependent() {
  int a[10];
  // test this again to ensure we skip properly during instantiation.
  [[intel::ivdep(3)]]
  // expected-warning@-1 2{{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen 5 >= safelen 3}}
  // expected-note@+1 2{{previous attribute is here}}
  [[intel::ivdep(5)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  [[intel::ivdep(C)]]
  // expected-error@-1 {{'ivdep' attribute requires a positive integral compile time constant expression}}
  for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-warning@+3 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen 4 >= safelen 2}}
  // expected-note@+1 {{previous attribute is here}}
  [[intel::ivdep(A)]]
  [[intel::ivdep(B)]]
  // expected-warning@-2 {{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen 4 >= safelen 2}}
  // expected-note@-2 {{previous attribute is here}}
  for (int i = 0; i != 10; ++i)
      a[i] = 0;

  (void)[]() {
  // expected-warning@+3 2{{ignoring redundant Intel FPGA loop attribute 'ivdep': safelen INF >= safelen INF}}
  // expected-note@+1 2{{previous attribute is here}}
  [[intel::ivdep]]
  [[intel::ivdep]] while (true);
  };
}

template <int A, int B, int C>
void ii_dependent() {
  int a[10];
  // expected-error@+1 {{'ii' attribute requires a positive integral compile time constant expression}}
  [[intel::ii(C)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-error@+1 {{duplicate Intel FPGA loop attribute 'ii'}}
  [[intel::ii(A)]]
  [[intel::ii(B)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
}

template <int A, int B, int C>
void max_concurrency_dependent() {
  int a[10];
  // expected-error@+1 {{'max_concurrency' attribute requires a non-negative integral compile time constant expression}}
  [[intel::max_concurrency(C)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;

  // expected-error@+1 {{duplicate Intel FPGA loop attribute 'max_concurrency'}}
  [[intel::max_concurrency(A)]]
  [[intel::max_concurrency(B)]] for (int i = 0; i != 10; ++i)
      a[i] = 0;
}

template <typename name, typename Func>
__attribute__((sycl_kernel)) void kernel_single_task(const Func &kernelFunc) {
  kernelFunc();
}

int main() {
  kernel_single_task<class kernel_function>([]() {
    foo();
    foo_deprecated();
    boo();
    goo();
    zoo();
    loop_attrs_compatibility();
    ivdep_dependent<4, 2, 1>();
    //expected-note@-1 +{{in instantiation of function template specialization}}
    ivdep_dependent<2, 4, -1>();
    //expected-note@-1 +{{in instantiation of function template specialization}}
    ii_dependent<2, 4, -1>();
    //expected-note@-1 +{{in instantiation of function template specialization}}
    max_concurrency_dependent<1, 4, -2>();
    //expected-note@-1 +{{in instantiation of function template specialization}}
  });
  return 0;
}
