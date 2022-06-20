/* Demo call user jl module with array I/O from C via @cfunction, with timer.
Barnett 2/2/21

Compile with (change gcc-9 to your gcc):

export JULIA_DIR=/usr/local/julia-1.5.2/
gcc-9 -Wall -g -o cfuncmodarr -fPIC cfuncmodarr.c -I$JULIA_DIR/include/julia -L$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib -Wl,-rpath=$JULIA_DIR/lib/julia -ljulia
./cfuncmodarr

Output should be:
filling array a...
err:0.0
julia set-up time 0.654 s
cfunction to julia ArrMod func foomp2 done in 0.261 s
max err = 8.88e-16  (single-threaded C took 1.69 s)

*/

#include <julia.h>
JULIA_DEFINE_FAST_TLS

#include <stdio.h>
#include <math.h>
#include <time.h>

typedef void (*myfun_ptr)(double*,double*,int);     // ptr type for func
myfun_ptr myfun = NULL;                             // instantiate a ptr

int main()
{
  int n=(int)1.0e8;                                 // make some data
  printf("filling array a...\n");
  double *a = (double*)malloc(sizeof(double)*n);
  for (int j=0;j<n;++j)
    a[j] = 1.0 + (double)j/n;                 // make some vals in array
  double *b = (double*)malloc(sizeof(double)*n);    // alloc output array

  struct timespec t0,t1;
  clock_gettime(CLOCK_REALTIME, &t0);
  
  jl_init();                                  // setup wrapping from C:
  jl_eval_string("push!(LOAD_PATH,\".\")");   // so can use a local module
  jl_eval_string("using ArrMod");
  jl_eval_string("a=rand(10); b=similar(a); foomp2_wrap(pointer(a),pointer(b),10); println(\"err:\",maximum(abs.(b-exp.(a))))");    // optional: test in pure jl
  jl_value_t *ret = jl_eval_string("@cfunction(foomp2_wrap, Cvoid, (Ptr{Cdouble},Ptr{Cdouble},Cint))");
  // note following conversion of ret must happen before any other jl_* calls!
  myfun = (myfun_ptr) jl_unbox_voidpointer(ret);    // convert to C func ptr
  
  clock_gettime(CLOCK_REALTIME, &t1);
  double t=(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1.0e9;
  printf("julia set-up time %.3g s\n",t);

  clock_gettime(CLOCK_REALTIME, &t0);
  myfun(a,b,n);                                     // call it, writes to b
  clock_gettime(CLOCK_REALTIME, &t1);
  t=(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1.0e9;
  printf("cfunction to julia ArrMod func foomp2 done in %.3g s\n",t);
  
  // check the result in b...
  clock_gettime(CLOCK_REALTIME, &t0);
  double maxerr = 0.0;
  for (int j=0;j<n;++j) {
    double e = fabs(exp(a[j])-b[j]);
    if (e>maxerr) maxerr=e;
  }
  clock_gettime(CLOCK_REALTIME, &t1);
  t=(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1.0e9;
  printf("max err = %.3g  (vs single-threaded C took %.3g s)\n",maxerr,t);
  
  jl_atexit_hook(0);                   // clean up
  free(a); free(b);
  return 0;
}
