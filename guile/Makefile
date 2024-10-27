SHELL := bash

FFI_DIR = ./ffi
SCHEME_BINDINGS = $(FFI_DIR)/libpd.scm
FFI_SRC = $(FFI_DIR)/libpd.ffi

.PHONY: default clean

default: $(SCHEME_BINDINGS)

$(SCHEME_BINDINGS): $(FFI_SRC)
	./gen-ffi.sh

clean:
	rm -f $(SCHEME_BINDINGS)
