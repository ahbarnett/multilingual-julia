## Example of Fortran calling Julia.

This uses the route F -> C -> Julia. Thus all of the comments in the `ccallj` directory apply. Multithreading on the julia side works without any special attention on the F side.

Use `make` to build and run.

### Notes

### Examples

`modulearr` : passes existing F array, writes to array preallocated on the F side. The julia func called is `foomp2` from the julia module `ArrModF` (this is essentially the same as the one in `ccallj`). The Fortran driver is `modulearr.f90`, and `cwrapper.c` is the intermediate C layer, just a hack for now.  
