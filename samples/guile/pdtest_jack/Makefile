JACK_FFI_SRC = ./ffi/jack.ffi
JACK_SCM = ./ffi/jack.scm

GUILE_LIBPD_DIR = ../../../guile

default: $(JACK_SCM) guile-libpd

$(JACK_SCM): $(JACK_FFI_SRC)
	./gen-ffi.sh

guile-libpd:
	cd $(GUILE_LIBPD_DIR) && make

.PHONY: clean default guile-libpd

clean:
	rm -f $(JACK_SCM)

deep-clean:
	$(MAKE) -C $(GUILE_LIBPD_DIR) clean
