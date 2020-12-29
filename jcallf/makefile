-include ../make.inc

default: demo1 demomp

demo1: demo1.so
	julia demo1.jl
demo1.so: demo1.f
	$(FC) demo1.f -o demo1.so -shared -fPIC

demomp: demomp.so
	julia demomp.jl
demomp.so: demomp.f90
	$(FC) -O2 demomp.f90 -o demomp.so -shared -fPIC -fopenmp

