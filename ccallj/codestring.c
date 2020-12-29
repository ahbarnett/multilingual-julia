// Test calling jl from C, etc. From manual. See makefile.  Barnett 12/28/20

#include <julia.h>
#include <stdio.h>

JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

int main(int argc, char *argv[])
{
    /* required: setup the Julia context */
    jl_init();

    /* run Julia commands; this gives text output but not passing values */
    jl_eval_string("a=2.0");
    jl_eval_string("println(sqrt(a))");  // note this continues the jl session

    printf("the above should be julia printing sqrt(2)\n");

    /* strongly recommended: notify Julia that the
         program is about to terminate. this allows
         Julia time to cleanup pending write requests
         and run all finalizers
    */
    jl_atexit_hook(0);
    return 0;
}
