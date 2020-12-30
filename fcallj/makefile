# Makefile for example calling julia from Fortran.
include ../make.inc

# linking paths and runtime lib... (same as for C driver)
LDFL=-I$(JULIA_DIR)/include/julia -L$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib/julia -ljulia

# executables
E=modulearr

# having executables here forces rebuild & rerun
.PHONY: $(E) testjulia clean

default: $(E)

modulearr: modulearr.f90 ArrModF.jl cwrapper.o
# note linking order: f90 needs cwrapper which needs -ljulia, so precede them
	$(FC) -o $@ -fPIC $< cwrapper.o -lstdc++ $(LDFL)
	./$@

cwrapper.o: cwrapper.c
	$(CC) -c $< -fPIC -I$(JULIA_DIR)/include/julia

testjulia:
	julia test_arrmodf.jl

clean:
	rm -f $(E) *.o
