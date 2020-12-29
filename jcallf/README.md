## Examples of julia calling Fortran.

These use the `ccall` julia function. It's very easy, although care over
`Ref` or passing pointers (eg, via everything being an array) is needed.

Run `make` to build and run.

### Notes

* If using the REPL, changes in the shared object are not recognized until the
REPL is restarted

### Examples

`demo1` : scalar and array float I/O  
`demomp` : array I/O with OpenMP  

