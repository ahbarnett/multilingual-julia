JULIA_DIR=/usr/local/julia-1.5.2

# linking paths and runtime lib...
LDFL=-I$(JULIA_DIR)/include/julia -L$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib -Wl,-rpath=$(JULIA_DIR)/lib/julia -ljulia

-include ../make.inc

.PHONY: codestring basefunc basearr

default: codestring basefunc basearr

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
	rm -f codestring basefunc basearr
