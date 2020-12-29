## Examples of C calling Julia.

This follows the julialang docs,
https://docs.julialang.org/en/v1/manual/embedding/

Use `make` to build and run.

Notes:

* `rpath` needed an extra `lib/julia` so `libLLVM-9jl.so` could be found at linking. See `makefile`

* Boxing/unboxing is awkward, in the style of MEX wrappers, and low-level garbage collector macros needed to prevent GC from
freeing pointers to jl returned values (boxed ones).

### Examples

`codestring` : call single line of julia code, no passing of values; straight from the manual  
`basefunc` : pass scalar input and output to a single existing (Base) julia function; from manual
`basearr` : array I/O to existing (Base) julia func; similar to manual 30.6


