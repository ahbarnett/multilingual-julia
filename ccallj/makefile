JULIA_DIR=/usr/local/julia-1.5.2

# linking paths and runtime lib...
LDFL=-I$(JULIA_DIR)/include/julia -L$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib/julia -ljulia

-include ../make.inc

# executables
E=codestring basefunc basearr

.PHONY: $(E)

default: $(E)

codestring: codestring.c
	$(CC) -o codestring -fPIC codestring.c $(LDFL)
	./codestring

basefunc: basefunc.c
	$(CC) -o basefunc -fPIC basefunc.c $(LDFL)
	./basefunc

basearr: basearr.c
	$(CC) -o basearr -fPIC basearr.c $(LDFL) -lm
	./basearr

clean:
	rm -f $(E)
