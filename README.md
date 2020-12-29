# multilingual-julia

Examples of interfacing between julia and low-level languages.
Taken from julia docs, StackOverflow, etc, by Alex Barnett.
Tested in a linux environment only (Ubuntu 16.04 LTS plus GCC9, julia 1.5.2).

### Directories

`jcallf` : julia calls Fortran (wrapping functions in low-level language)  
`ccallj` : C calls julia ("embedding")  

Each directory has its own makefile. They all read any common settings placed in
 `make.inc` (eg copy `make.inc.example` to this file). Some demos explore multithreading.

### To do

* F calling J, via C wrappers I guess

* J calling F with non-void return type, eg `ddot` LAPACK in manual 27.5

* J calling C, eg w/ SIMD (manual has it); 27.14 thread safety I don't get

* understand garbage collection in the C calling J case (Manual 30.5)

* passing function handles in either direction (probably hard)

