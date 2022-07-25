.PHONY: all

default: all

D=jcallf ccallj fcallj pcallj

all:
	for i in $D; do echo "=============== $$i: ==============="; \
	(cd $$i; make); done
clean:
	for i in $D; do (cd $$i; make clean); done
