JULIA_DIR=/usr/local/julia-1.5.2

# linking paths and runtime lib...
LDFL=-I$(JULIA_DIR)/include/julia -L$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib/julia -ljulia

-include ../make.inc

# executables
E=codestring basefunc basearr modulearr modulearr2

.PHONY: $(E)

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

clean:
	rm -f $(E)
