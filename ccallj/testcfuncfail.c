/* min complete example of C calling J w/ array. Barnett 2/1/21.
   compile/test with:

export JULIA_DIR=/usr/local/julia-1.5.2/
gcc-9 -o testcfuncfail -fPIC testcfuncfail.c -I$JULIA_DIR/include/julia -L$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib/julia -ljulia
./testcfuncfail

*/

#include <julia.h>
typedef void (*test2_ptr)(double*, int);
test2_ptr test2 = NULL;

int main()
{
  jl_init();
  jl_eval_string("function test2(x,n) println(maximum(x[1:n])) end");
  jl_eval_string("x=rand(10); test2(x,6)");  // test it from jl side 
  jl_value_t *ret = jl_eval_string("@cfunction(test2, Cvoid, (Ref{Cdouble}, Cint))");
  test2 = (test2_ptr) jl_unbox_voidpointer(ret);     // convert to C func ptr
  double a[4] = {1.0,2.0,7.0,3.0};             // input data
  test2(a,4);
  jl_atexit_hook(0);
  return 0;
}
