// Calling jl from C, existing function in julia. From manual. Barnett 12/28/20

#include <stdio.h>
#include <julia.h>

JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

int main(int argc, char *argv[])
{
    /* required: setup the Julia context */
    jl_init();

    double a = 2.0;
    
    // call a base func from julia...
    jl_function_t *func = jl_get_function(jl_base_module, "sqrt");
    jl_value_t *argument = jl_box_float64(a);
    jl_value_t *ret = jl_call1(func, argument);  // 1 means #args, convenience

    double ret_unboxed = jl_unbox_float64(ret);   // must unbox it
    printf("ret = %.15g (should be sqrt(2))\n",ret_unboxed);
    
    /* strongly recommended: notify Julia that the
         program is about to terminate. this allows
         Julia time to cleanup pending write requests
         and run all finalizers
    */
    jl_atexit_hook(0);
    return 0;
}
