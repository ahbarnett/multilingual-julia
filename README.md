# multilingual-julia

Minimally complete examples of interfacing between Julia and low-level languages, including the uncommon case of _Fortran calling Julia_.
Taken from Julia docs, StackOverflow, etc, by Alex Barnett.
Tested in a linux environment only (Ubuntu 16.04 LTS plus GCC9, Julia 1.5.2).


### Contents

`jcallf` : Julia calls Fortran (ie, wrapping low-level language functions)  
`ccallj` : C calls Julia ("embedding")  
`fcallj` : Fortran calls Julia ("embedding", but needs intermediate C layer)  

Each directory has its own makefile. They all read any common settings placed in `make.inc` (eg copy `make.inc.example` to this file). Some demos explore multithreading in the "inner" (called) function. Some also report timings (which adds a few lines of code beyond the "minimal").


### Requirements

Julia installed, C complier, Fortran compiler, GNU `make`.


### Testing

`cp make.inc.example make.inc`

Edit `make.inc` for your Julia installation directory and choice of compilers

`make`

This goes to each directory and makes/runs all examples. Total time is around 10 seconds.


### To do

* J calling F with non-void return type, eg `ddot` LAPACK in manual 27.5

* J calling C, eg w/ SIMD (manual has it); 27.14 thread safety I don't get

* understand garbage collection in the C calling J case (Manual 30.5)

* passing function handles in either direction (probably hard)

* more general C wrapper utilities for F calling J funcs with simple array args
