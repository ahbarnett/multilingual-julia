/* Demo C wrapper layer for Fortran to call specific julia func ("foomp2"),
   which lives in a local module. All is hardwired for now. Barnett 2/3/21

   3 stages are:
   julia_setup (opens a julia session, sends text cmds, gets func ptrs)
   julia_foomp2 (calls example julia function)
   julia_cleanup (closes julia session)

   See makefile for build. Note this wrapper would be useful for C calling J too
*/

#include <julia.h>
JULIA_DEFINE_FAST_TLS()

#include <stdio.h>

typedef void (*foomp2_ptr)(double*,double*,int);     // ptr type for foomp2 func
foomp2_ptr foomp2 = NULL;                            // instantiate a ptr

void julia_setup_(int64_t* ierptr)
// start up julia, set pointers to wrapped functions. Passes out 0 if success,
// nonzero otherwise.
{
  *ierptr = 0;     // no error yet
  jl_init();                                  // setup wrapping from C:
  jl_eval_string("push!(LOAD_PATH,\".\")");   // so can use a local module
  jl_eval_string("using ArrModF");
  jl_value_t *ret = jl_eval_string("@cfunction(foomp2_wrap, Cvoid, (Ptr{Cdouble},Ptr{Cdouble},Cint))");
  foomp2 = (foomp2_ptr) jl_unbox_voidpointer(ret);    // convert to C func ptr
  // note this processing of ret value must happen before any other jl_* call, including the exception checking, else seemingly segfaults!
  // printf("foomp2 ptr = %p\n",foomp2);      // debug
  if (jl_exception_occurred()) {
    printf("Error in %s cfunction: %s \n", __func__,jl_typeof_str(jl_exception_occurred()));
    *ierptr=1;      // error code
    return;
  }
  // optional: test on jl side (slows the setup but warms up, ie compiles, this julia func)...
  jl_eval_string("n=Cint(10); a=rand(n); b=similar(a); foomp2_wrap(pointer(a),pointer(b),n); println(\"julia_setup foomp2 test err:\",maximum(abs.(b-exp.(a))))");
  if (jl_exception_occurred()) {
    printf("Error in %s jl-side test: %s \n", __func__,jl_typeof_str(jl_exception_occurred()));
    *ierptr=1;      // error code
    return;
  }
}

void julia_foomp2_(double* x, double* y, int64_t* nptr)
// F->C wrapper for the previously-pointed-to julia func foomp2. No error check.
// You would need one of these wrappers per julia function.
{
  foomp2(x,y,(int)*nptr);         // C expects value for n, not ptr like fort
}

void julia_cleanup_()
{
  jl_atexit_hook(0);
}
