/* Demo calling a native jl func ("sqrt") from C via @cfunction. Barnett 2/2/21
Compile with (change gcc-9 to your gcc):

export JULIA_DIR=/usr/local/julia-1.5.2/
gcc-9 -Wall -g -o cfunction1 -fPIC cfunction1.c -I$JULIA_DIR/include/julia -L$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib/julia -ljulia -lm
./cfunction1

*/

#include <julia.h>
JULIA_DEFINE_FAST_TLS()
#include <math.h>

typedef double (*myfun_ptr)(double);         // ptr type for func's C signature
myfun_ptr myfun = NULL;                      // instantiate a ptr

int main()
{
  jl_init();
  jl_value_t *ret = jl_eval_string("@cfunction(sqrt, Cdouble, (Cdouble,))");
  myfun = (myfun_ptr) jl_unbox_voidpointer(ret);    // convert to C func ptr
  double x = 2.0;
  double y = myfun(x);                              // call it
  printf("%s: x=%.16g, y=%.16g (error: %.3g)\n",__FILE__,x,y,y-sqrt(x));  // chk
  jl_atexit_hook(0);
  return 0;
}
