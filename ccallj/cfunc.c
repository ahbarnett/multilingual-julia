// Calling jl from C, user-written julia func, array I/O. Barnett 2/1/21
// Version 3: S G Johnson @cfunction, output array is alloc by C, written into.
// See: https://scicomp.stackexchange.com/questions/23194/i-am-searching-for-a-c-code-implementing-the-complex-polygamma-function/23733#23733
// And, for a flavor of cfunction: https://docs.julialang.org/en/v1/manual/calling-c-and-fortran-code/
// https://stackoverflow.com/questions/65322339/how-to-use-julia-special-functions-inside-c

// *** doesn't work



#include <julia.h>
JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

#include <stdio.h>
#include <math.h>
#include <time.h>

// set up typedef for how wrapped func appears from C side
typedef void (*foomp3_ptr)(double *, double *, int);
// instantiate global ptr to our wrapped func
foomp3_ptr foomp3 = NULL;

int main(int argc, char *argv[])
{
  jl_init();        // required: setup the Julia context
  
  int n=100000000;
  printf("filling array a...\n");
  double *a = (double*)malloc(sizeof(double)*n);
  for (int j=0;j<n;++j)
    a[j] = 1.0 + (double)j/n;             // make some vals in array
  double *b = (double*)malloc(sizeof(double)*n);    // alloc output array
  
  struct timespec t0,t1;
  clock_gettime(CLOCK_REALTIME, &t0);
  
  // get our julia module via string commands to jl session...
  jl_eval_string("push!(LOAD_PATH,\".\")");   // so can use local module
  jl_eval_string("using ArrMod");            // our module
  // jl_eval_string("n=10; a=rand(10); b=a; foomp3(rand(n),rand(n),n)"); // checl it, ok
  jl_value_t *ret = jl_eval_string("@cfunction(foomp3, Cvoid, (Ptr{Cdouble}, Ptr{Cdouble}, Cint))");     // signature wrapping done on jl side
  // manual 30.7...  (seems to report MethodError if scalar exp tried)
  foomp3 = (foomp3_ptr) jl_unbox_voidpointer(ret);     // convert to C func ptr
  if (jl_exception_occurred())
    printf("cfunc, uh-oh: %s \n", jl_typeof_str(jl_exception_occurred()));
  clock_gettime(CLOCK_REALTIME, &t1);
  double t=(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1.0e9;
  printf("julia set-up time %.3g s\n",t);
  
  clock_gettime(CLOCK_REALTIME, &t0);
  foomp3(a,b,n);                           // call it, writes to b
  clock_gettime(CLOCK_REALTIME, &t1);
  t=(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1.0e9;
  printf("call to julia @cfunction ArrMod foomp2 done in %.3g s\n",t);
  
  // check b
  clock_gettime(CLOCK_REALTIME, &t0);
  double maxerr = 0.0;
  for (int j=0;j<n;++j) {
    double e = fabs(exp(a[j])-b[j]);   // checks the "exp" func, fails
    if (e>maxerr) maxerr=e;
  }
  clock_gettime(CLOCK_REALTIME, &t1);
  t=(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1.0e9;
  printf("max err = %.3g  (single-threaded C took %.3g s)\n",maxerr,t);
  
  jl_atexit_hook(0);
  
  free(a); free(b);
  return 0;
}
