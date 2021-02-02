/* Calling a scalar I/O user jl func from C via @cfunction. Barnett 2/2/21
Compile with (change gcc-9 to your gcc):

export JULIA_DIR=/usr/local/julia-1.5.2/
gcc-9 -Wall -g -o democfunction2 -fPIC democfunction2.c -I$JULIA_DIR/include/julia -L$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib/julia -ljulia -lm
./democfunction2

*/

#include <julia.h>
JULIA_DEFINE_FAST_TLS()
#include <math.h>

typedef double (*myfun_ptr)(double);         // ptr type for func's C signature
myfun_ptr myfun = NULL;                      // instantiate a ptr

int main()
{
  jl_init();
  // double in, returns double. Adds text output on jl side...
  jl_eval_string("function myfun(x) println(\"in Julia, x=\",x); sqrt(x) end");
  jl_eval_string("println(myfun(2.0))");            // optional: test it from jl
  jl_value_t *ret = jl_eval_string("@cfunction(myfun, Cdouble, (Cdouble,))");
  myfun = (myfun_ptr) jl_unbox_voidpointer(ret);    // convert to C func ptr
  double x = 2.0;
  double y = myfun(x);                              // call it
  printf("%s: x=%.16g, y=%.16g (error: %.3g)\n",__FILE__,x,y,y-sqrt(x));  // chk
  jl_atexit_hook(0);
  return 0;
}
