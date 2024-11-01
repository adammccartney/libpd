(use-modules (system foreign-library)
             (ffi libpd))

;;(load-foreign-library
;; (string-append
;;  (getenv "GUIX_ENVIRONMENT") "/lib/libpd.so"))

(libpd_init)
