## Examples of C calling Julia.

This follows the julialang docs,
https://docs.julialang.org/en/v1/manual/embedding/

Use `make` to build and run all.

### Notes

* `rpath` needed an extra `lib/julia` so `libLLVM-9jl.so` could be found at linking. See `makefile`

* Boxing/unboxing is awkward, in the style of MEX wrappers, and low-level garbage collector macros needed to prevent GC from
freeing pointers to jl returned values (boxed ones).

* Cannot figure out how to call a function from a user-written julia module.
`julia.h` is not much use, and is super complicated. Eg here's a comment from
it:
```
// This type is a placeholder to cache data for a specType signature specialization of a Method
// can can be used as a unique dictionary key representation of a call to a particular Method
// with a particular set of argument types
```
Yikes!

Others have same qu; answered beautifully here:

https://stackoverflow.com/questions/55970064/calling-local-julia-package-from-c

* Why doing alloc on julia side fails in multithreaded case?

* You will need to set env var `JULIA_NUM_THREADS` to access multithreading on the julia side.


### Examples

`codestring` : call single line of julia code, no passing of values; straight from the manual  
`basefunc` : pass scalar input and output to a single existing (Base) julia function; from manual  
`basearr` : array I/O to existing (Base) julia func; similar to manual 30.6  
`modulearr` : array I/O to user julia func in module; from above SO post. J allocs the output array. Fails when multithreaded (why?)
`modulearr2` : same except Fortran-style, output array preallocated on the C side. Works multithreaded  

