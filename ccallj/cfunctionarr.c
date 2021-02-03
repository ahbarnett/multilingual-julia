/* Demo calling a user jl func with array from C via @cfunction. Barnett 2/2/21
Compile with (change gcc-9 to your gcc):

export JULIA_DIR=/usr/local/julia-1.5.2/
gcc-9 -Wall -g -o cfunctionarr -fPIC cfunctionarr.c -I$JULIA_DIR/include/julia -L$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib/julia -ljulia
./cfunctionarr

Output should be:
0.9694413805077366
7.0
cfunctionarr.c: should have printed 7.0

*/

#include <julia.h>
JULIA_DEFINE_FAST_TLS()

typedef void (*myfun_ptr)(double*, int);     // ptr type for func's C signature
myfun_ptr myfun = NULL;                      // instantiate a ptr

int main()
{
  jl_init();
  // Array in, void out, just prints. Can add @show in jl func to chk objects...
  jl_eval_string("function myfun(xptr,n) x=unsafe_wrap(Array,xptr,(n,); own=false); println(maximum(x)); return nothing; end");
  // (note void return value is essential to match @cfunction signature)
  jl_eval_string("x=rand(10); myfun(pointer(x),6)");    // first test it in jl
  jl_value_t *ret = jl_eval_string("@cfunction(myfun, Cvoid, (Ptr{Cdouble},Cint))");
  myfun = (myfun_ptr) jl_unbox_voidpointer(ret);    // convert to C func ptr
  double a[4] = {1.0,2.0,7.0,3.0};                  // input array
  myfun(a,4);                                       // call it
  printf("%s: should have printed 7.0\n",__FILE__); // chk by eye
  jl_atexit_hook(0);
  return 0;
}
