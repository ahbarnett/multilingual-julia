// Calling jl from C: simple funcs via @cfunction. Barnett 2/1/21

// See: https://scicomp.stackexchange.com/questions/23194/i-am-searching-for-a-c-code-implementing-the-complex-polygamma-function/23733#23733
// And, for a flavor of cfunction: https://docs.julialang.org/en/v1/manual/calling-c-and-fortran-code/
// https://stackoverflow.com/questions/65322339/how-to-use-julia-special-functions-inside-c

#include <julia.h>
JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

#include <stdio.h>
#include <math.h>

// set up typedef for how wrapped func appears from C side
typedef double (*test_ptr)(double);
// instantiate global ptr to our wrapped func
test_ptr test = NULL;

typedef void (*test1_ptr)(double);
test1_ptr test1 = NULL;

typedef void (*test2_ptr)(double*,int);
test2_ptr test2 = NULL;

typedef void (*myexp2_ptr)(double*,double*,int);
myexp2_ptr myexp2 = NULL;

int main(int argc, char *argv[])
{
  jl_init();        // required: setup the Julia context

  // wrap Julia's base "exp" function...
  jl_value_t *ret = jl_eval_string("@cfunction(exp, Cdouble, (Cdouble,))");
  test = (test_ptr) jl_unbox_voidpointer(ret);     // convert to C func ptr
  double x = 1.0;
  printf("native jl func double in, double out: x=%g, y=%g (should be %g)\n\n",x,test(x),exp(x));

  // now wrap Julia's own instance of "exp" function with same signature...
  jl_eval_string("function myexp(x) exp(x) end");
  ret = jl_eval_string("@cfunction(myexp, Cdouble, (Cdouble,))");
  test = (test_ptr) jl_unbox_voidpointer(ret);     // convert to C func ptr
  printf("user jl func double in, double out: x=%g, y=%g (should be %g)\n\n",x,test(x),exp(x));

  // now wrap a func returning void... just prints the value
  jl_eval_string("function test1(x) println(x) end");
  ret = jl_eval_string("@cfunction(test1, Cvoid, (Cdouble,))");
  test1 = (test1_ptr) jl_unbox_voidpointer(ret);     // convert to C func ptr
  x = 1.234;
  test1(x);
  printf("should have printed %g\n\n",x);

  // now wrap a func returning void and taking in array of given length...
  //jl_eval_string("function test2(x::Array{Float64,1},n::Int64) println(maximum(x[1:n])) end");
  jl_eval_string("function test2(x,n) println(maximum(x[1:n])) end");
  ret = jl_eval_string("@cfunction(test2, Cvoid, (Ref{Cdouble}, Cint))");
  test2 = (test2_ptr) jl_unbox_voidpointer(ret);     // convert to C func ptr
  double a[4] = {1.0,2.0,7.0,3.0};
  test2(a,4);
  printf("should have printed 7.0\n\n");

  
  // the previous fails, but I'd also like to pass out like this:
  
  // now try array version... need to pass length n from C side...
  jl_eval_string("function myexp2(x,y,n) y[1:n] = exp.(x[1:n]) end");
  ret = jl_eval_string("@cfunction(myexp2, Cvoid, (Ref{Cdouble},Ref{Cdouble},Cint))");
  myexp2 = (myexp2_ptr) jl_unbox_voidpointer(ret);     // convert to C func ptr
  double b[4];                      // output alloc
  myexp2(a,b,4);                    // call it
  printf("user jl func w/ arrays: a[3]=%g, b[3]=%g (should be %g)\n\n",a[3],b[3],exp(a[3]));

  jl_atexit_hook(0);
  return 0;
}
