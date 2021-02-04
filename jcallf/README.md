## Examples of julia calling Fortran.

These use the `ccall` julia function. It's very easy, although care over
`Ref` or passing pointers (eg, via making a length-1 array on the Julia side) is needed.

Run `make` to build and run.

### Examples

* `demo1` : scalar and array float I/O
* `demomp` : array I/O with OpenMP

### Notes

* If using the REPL, changes in the shared object are not recognized until the
REPL is restarted

* Julia calling Fortran demos by Daniel Crespo: https://github.com/zyth0s/SciAlgs.jl/tree/master/src/interface_fortran

