/* min complete example of C calling J, passing array. Barnett 2/1/21.
   compile/test with:

export JULIA_DIR=/usr/local/julia-1.5.2/
gcc-9 -Wall -g -o testcfuncfail -fPIC testcfuncfail.c -I$JULIA_DIR/include/julia -L$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib/julia -ljulia
./testcfuncfail

*** Fails currently at the call step - to fix!
Note the similar scalar tests in testcfunc.c work fine.

*/

#include <julia.h>
typedef void (*test2_ptr)(double*, int);             // globally define func ptr obj
test2_ptr test2 = NULL;

int main()
{
  jl_init();
  //jl_eval_string("function test2(x,n) println(maximum(x[1:n])) end");
  //jl_eval_string("function test2(xptr,n) @show xptr; @show n; y=unsafe_wrap(Cdouble,xptr,(n,)); println(maximum(y)) end");
  jl_eval_string("function test2(xp,n) @show xp; @show n; x=unsafe_wrap(Array,xp,(n,)); @show x; println(maximum(x)); return nothing; end");
  jl_eval_string("x=rand(10); test2(Ref(x),6)");          // test it from jl side
  jl_value_t *ret = jl_eval_string("@cfunction(test2, Cvoid, (Ptr{Cdouble}, Cint))");
  test2 = (test2_ptr) jl_unbox_voidpointer(ret);     // convert to global C func ptr
  double a[4] = {1.0,2.0,7.0,3.0};                   // input data, static
  test2(a,4);                                        // should print 7.0
  jl_atexit_hook(0);
  return 0;
}
