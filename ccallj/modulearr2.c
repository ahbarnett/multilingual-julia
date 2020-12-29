// Calling jl from C, user-written julia func, array I/O. Barnett 12/28/20
// From nice StackOverflow answer of Hamza Yusuf Çakır (hckr).  Also times it.
// Version 2: output array is alloc by C, written into.

#include <stdio.h>
#include <julia.h>
#include <math.h>
#include <time.h>

JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

int main(int argc, char *argv[])
{
    /* required: setup the Julia context */
    jl_init();

    int n=100000000;
    printf("filling array a...\n");
    double *a = (double*)malloc(sizeof(double)*n);
    for (int j=0;j<n;++j)
      a[j] = 1.0 + (double)j/n;             // make some vals in array
    double *b = (double*)malloc(sizeof(double)*n);    // alloc output array
    
    // make array type
    jl_value_t* arr_type = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);  // here 1 is #dims
    int owner = 0;        // so C responsible for freeing it
    jl_array_t *aj = jl_ptr_to_array_1d(arr_type, a, n, owner);   // jl ptr
    jl_array_t *bj = jl_ptr_to_array_1d(arr_type, b, n, owner);   // jl ptr

    // get our julia module via string commands to jl session...
    struct timespec t0,t1;
    clock_gettime(CLOCK_REALTIME, &t0);
    jl_eval_string("push!(LOAD_PATH,\".\")");   // so can use local module
    jl_eval_string("using ArrMod");
    jl_module_t* mod = (jl_module_t *)jl_eval_string("ArrMod");   // ptr to it
    // get our func
    jl_function_t *func = jl_get_function(mod, "foomp2");  // foo or foomp
    clock_gettime(CLOCK_REALTIME, &t1);
    double t=(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1.0e9;
    printf("julia set-up time %.3g s\n",t);
    
    clock_gettime(CLOCK_REALTIME, &t0);
    jl_call2(func, (jl_value_t*)aj, (jl_value_t*)bj);  // writes to bj
    clock_gettime(CLOCK_REALTIME, &t1);
    t=(t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1.0e9;
    printf("call to julia ArrMod func foomp2 done in %.3g s\n",t);
    
    // manual 30.7...  (seems to report MethodError if scalar exp tried)
    if (jl_exception_occurred())
      printf("uh-oh: %s \n", jl_typeof_str(jl_exception_occurred()));
    
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
    
    /* strongly recommended: notify Julia that the
         program is about to terminate. this allows
         Julia time to cleanup pending write requests
         and run all finalizers
    */
    jl_atexit_hook(0);

    free(a); free(b);
    return 0;
}
