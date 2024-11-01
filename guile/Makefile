SHELL := bash

FFI_DIR = ./ffi
GUILE_LIBPD_SRC = $(FFI_DIR)/libpd.ffi
GUILE_LIBPD_SCM = $(patsubst %.ffi,%.scm,$(GUILE_LIBPD_SRC))

.PHONY: default clean

default: $(GUILE_LIBPD_SCM)

$(GUILE_LIBPD_SCM): $(GUILE_LIBPD_SRC)
	./gen-ffi.sh

clean:
	rm -f $(GUILE_LIBPD_SCM)
