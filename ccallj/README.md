## Examples of C calling Julia.

There are two styles of calling Julia from C:

1) Julia calling convention, using a quite long set of `jl_*` calls in C. This
is the main route explained in the manual
https://docs.julialang.org/en/v1/manual/embedding/
It is clumsy (boxing/unboxing required), and gets moreso for complicated function signatures.

2) Packaging the Julia code on the Julia side as a `@cfunction` which can be accessed from C via a couple of `jl_*` calls. Although the signature must match, this is a much more lightweight approach, nearly as simple as one could hope for. However, it is documented for only one simple scalar case. We remedy that here.

We recommend 2; it is much neater.
This directory has minimally complete tutorial examples of both, starting simple then getting more complicated. Goals:

* accessing functions within a user-written Julia module  
* passing arrays (the allocation being on the C side)  
* using multithreading  

Use `make` to build and run all examples.


### Examples

The minimal module is called `ArrMod.jl` and its tester `test_arrmod.jl`.
Take a look: there is a function `foomp` (and its variants) that we want to access from C. Name derives from `foo` + OpenMP. It just does elementwise exp of an array.

Hello-world example:

* `codestring` : call single line of julia code, no passing of values; straight from the manual

Julia convention (style 1):

* `basefunc` : pass scalar input and output to a single existing (Base) julia function; from manual
* `basearr` : array I/O to existing (Base) julia func; similar to manual 30.6  
* `modulearr` : array I/O to user julia func in module; from above SO post. J allocs the output array. Fails only if multithreaded
* `modulearr2` : same, except Fortran-style, output array preallocated on the C side. Works multithreaded

Cfunction wrapping convention (style 2):

* `cfunction1` : basic call of native jl func with scalar I/O
* `cfunction2` : call user-defined jl func with scalar I/O
* `cfunctionarr` : call user-defined jl func, with array input
* `cfuncmodarr` : call user-defined jl func in a module, with array I/O, timing


### Notes

* `rpath` needed an extra `lib/julia` so `libLLVM-9jl.so` could be found at linking. See `makefile`

* You will need to set env var `JULIA_NUM_THREADS` to access multithreading on the julia side.

* Here is my thread on this (thanks to those who answered): https://discourse.julialang.org/t/i-made-examples-of-wrapping-jl-code-from-fortran-c-how-generalize/

* Here are some notes on the Julia calling convention (route 1):
  * https://stackoverflow.com/questions/55970064/calling-local-julia-package-from-c
  * https://discourse.julialang.org/t/passing-a-c-struct-into-julia/43965/16  
  * Boxing/unboxing is awkward, in the style of MEX wrappers, and low-level garbage collector macros needed to prevent GC from freeing pointers to jl returned values (boxed ones).  

* Here are some notes on the `@cfunction` approach (route 2):
  * For passing arrays (they use `unsafe_load` instead of my `unsafe_wrap`): https://github.com/GunnarFarneback/DynamicallyLoadedEmbedding.jl/pull/3  

* Reading `julia.h` is not much use, and is super complicated. Eg here's a comment from it:
```
// This type is a placeholder to cache data for a specType signature specialization of a Method
// can can be used as a unique dictionary key representation of a call to a particular Method
// with a particular set of argument types
```
Yikes!

* The runtime library loads in around 180 MB even for a hello-world example.
  To demo this, try `/usr/bin/time -f "%M" ./codestring` which displays
  the max RAM usage in kB, giving `178484` in my setup. This is huge.
  See: https://viralinstruction.com/posts/badjulia/


### Questions / To do

* In style 1, why doing alloc on julia side fails only in multithreaded case?
See `modulearr`; not urgent.

