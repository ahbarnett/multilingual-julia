## Example of Fortran calling Julia.

This uses the route F -> C -> Julia. Thus all of the comments in the `ccallj` directory apply. Multithreading on the julia side works without any special attention on the F side.

Use `make` to build and run.

### Examples

We skip the trivial examples and go straight to passing arrays, with multithreading, and timers in there too.
They wrap `foomp2` from the julia module `ArrModF` (which is essentially the same as the one in the `../ccallj` directory).

* `modulearr` : array I/O, with output array also allocated on the F side. The Fortran driver is `modulearr.f90`, and `jlfortwrapper.c` is the intermediate C layer. Recommended example.

* `modulearr_old_style1` : same goal as above, but more clunky code that uses in C the `jl_*` Julia convention interface. The intermediate layer is `cwrapper_old_style1.c`. This generalizes less well to more complex function signatures.


### Notes

* The above assumes classical Fortran name-mangling from C. A more universal
way to do this would be via the `ISO_C_BINDING` from 2003 onwards, and
`C_F_PROCPOINTER`:
http://www.lahey.com/docs/lfpro78help/gfortran/Working-with-Pointers.html
To investigate...
