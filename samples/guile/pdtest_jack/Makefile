JACK_FFI_SRC = ./ffi/jack.ffi
JACK_SCM = ./ffi/jack.scm

GUILE_LIBPD_DIR = ../../../

default: $(JACK_SCM) guile-libpd

$(JACK_SCM): $(JACK_FFI_SRC)
	./gen-ffi.sh

guile-libpd:
	$(MAKE) -C $(GUILE_LIBPD_DIR)

.PHONY: clean default guile-libpd

clean:
	rm -f $(JACK_SCM)

deep-clean:
	$(MAKE) -C $(GUILE_LIBPD_DIR) clean
