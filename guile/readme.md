guile-libpd
===========

This subproject is aimed at generating ffi bindings to libpd, so that it can be
used from within a guile scheme program. Note that it's quite linux-centric 
(i.e. it hasn't been tested on Windows/MAC).

The bindings are generated using the 'FFI Helper' from[nyacc](https://nongnu.org/nyacc).

To make life a bit easier, all dependencies are specified explicitly in
`manifest.scm`. This manifest can be fed to guix shell. You can see how this
environment gets set up in `gen-ffi.sh`. 

## Requirements

+ `guix`
+ `make`


## Buildling

Assuming that you have guix installed, things ar pretty straighforward:

To generate the bindings run:

`make`

This will build the file `ffi/libpd.scm`

To remove the file: 

`make clean`


