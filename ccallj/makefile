include ../make.inc

# linking paths and runtime lib...
LDFL=-I$(JULIA_DIR)/include/julia -L$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib/julia -ljulia

# executables
E=codestring basefunc basearr modulearr modulearr2 cfunction1 cfunction2 cfunctionarr cfuncmodarr

# having executables here forces rebuild & rerun
.PHONY: $(E) testjulia clean

default: $(E)

codestring: codestring.c
	$(CC) -o $@ -fPIC $< $(LDFL)
	./$@

basefunc: basefunc.c
	$(CC) -o $@ -fPIC $< $(LDFL)
	./$@

basearr: basearr.c
	$(CC) -o $@ -fPIC $< $(LDFL) -lm
	./$@

modulearr: modulearr.c ArrMod.jl
	$(CC) -o $@ -fPIC $< $(LDFL) -lm
	./$@

modulearr2: modulearr2.c ArrMod.jl
	$(CC) -o $@ -fPIC $< $(LDFL) -lm
	./$@

cfunction1: cfunction1.c
	$(CC) -o $@ -fPIC $< $(LDFL) -lm
	./$@

cfunction2: cfunction2.c
	$(CC) -o $@ -fPIC $< $(LDFL) -lm
	./$@

cfunctionarr: cfunctionarr.c
	$(CC) -o $@ -fPIC $< $(LDFL) -lm
	./$@

cfuncmodarr: cfuncmodarr.c ArrMod.jl
	$(CC) -o $@ -fPIC $< $(LDFL) -lm
	./$@

testjulia:
	julia test_arrmod.jl

clean:
	rm -f $(E)
