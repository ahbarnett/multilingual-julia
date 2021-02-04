// C wrapper utility to allow Fortran to call a specific julia func.
// Opens a julia session, sends text cmds, loads module then calls a func which
// does I/O to arrays allocated on the Fortran (calling) side.

// Has name of julia module and func name and arg types hardwired.

#include <julia.h>
JULIA_DEFINE_FAST_TLS()

#include <stdio.h>

void julia_foomp2_(double* x, double* y, int64_t* nptr, int64_t* ierptr)
// wrapper for specific julia func foomp2 in a certain module in current dir
{
  int n = *nptr;
  *ierptr = 0;     // success
  
  /* required: setup the Julia context */
  jl_init();

  // make jl array objects of our I & O...
  jl_value_t* arr_type = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);  // here 1 is #dims
  int owner = 0;        // so C responsible for its mem mgmt
  jl_array_t *xj = jl_ptr_to_array_1d(arr_type, x, n, owner);   // needs len
  jl_array_t *yj = jl_ptr_to_array_1d(arr_type, y, n, owner);   // needs len

  // hardwired name of module and func to wrap...
  jl_eval_string("push!(LOAD_PATH,\".\")");   // so can use local module in pwd
  jl_eval_string("using ArrModF");
  jl_module_t* mod = (jl_module_t *)jl_eval_string("ArrModF");   // ptr to it
  // get our func by name
  jl_function_t *func = jl_get_function(mod, "foomp2");

  // do it
  jl_call2(func, (jl_value_t*)xj, (jl_value_t*)yj);  // reads xj, writes to yj
  
  if (jl_exception_occurred()) {
    printf("Error in %s: %s \n", __func__,jl_typeof_str(jl_exception_occurred()));
    *ierptr=1;      // error code
  }

  /* strongly recommended: notify Julia that the
     program is about to terminate. this allows
     Julia time to cleanup pending write requests
     and run all finalizers
  */
  jl_atexit_hook(0);
}
