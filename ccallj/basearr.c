// Calling jl from C, base func in julia, array. Manual 30.6. Barnett 12/28/20
// Sends array a to native julia func, gets b back, checks it.

#include <stdio.h>
#include <julia.h>
#include <math.h>

JULIA_DEFINE_FAST_TLS // only define this once, in an executable (not in a shared library) if you want fast code.

int main(int argc, char *argv[])
{
    /* required: setup the Julia context */
    jl_init();

    int n=10000000;
    double *a = (double*)malloc(sizeof(double)*n);
    for (int j=0;j<n;++j)
      a[j] = 1.0 + (double)j/n;             // make some vals in array

    // make array type
    jl_value_t* a_arr_type = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);  // here 1 is #dims
    int owner = 0;        // so C responsible for freeing it
    jl_array_t *aj = jl_ptr_to_array_1d(a_arr_type, a, n, owner);   // jl ptr
    
    // call a base func from julia...   array in, array out
    jl_function_t *func = jl_get_function(jl_base_module, "reverse"); // man
    //jl_function_t *func = jl_get_function(jl_base_module, "exp"); // segfault
    jl_array_t *bj = (jl_array_t*)jl_call1(func, (jl_value_t*)aj); // jl alloc bj
    
    // manual 30.7...  (seems to report MethodError if scalar exp tried)
    if (jl_exception_occurred())
      printf("uh-oh: %s \n", jl_typeof_str(jl_exception_occurred()));

    // C ptr to returned array
    double *b = (double *)jl_array_data(bj);
    
    // check b
    double maxerr = 0.0;
    for (int j=0;j<n;++j) {
      // if (n-j<10) printf("b[%d]=%.15g\n",j,b[j]);  // examine elements
      double e = fabs(a[n-j-1]-b[j]);   // checks the "reverse" func
      //double e = fabs(exp(a[j])-b[j]);   // checks the "exp" func, fails
      if (e>maxerr) maxerr=e;
    }
    printf("max err = %.3g\n",maxerr);
    
    /* strongly recommended: notify Julia that the
         program is about to terminate. this allows
         Julia time to cleanup pending write requests
         and run all finalizers
    */
    jl_atexit_hook(0);

    free(a);
    return 0;
}
