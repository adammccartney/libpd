(use-modules (rnrs bytevectors)
             (system foreign)
             (ffi jack)
             (ffi libpd))

;; FIXME: find an appropriate data structure for this
;; @ref jack_options_t bits
(define jack_options
  (let ((bv (make-bytevector 7)))
    ;; Null value when no option bits are needed
    ;; jack_null_option
    (bytevector-u8-set! bv 0 #x00)
    ;; Don't automatically start if not needed
    ;; jack_no_start_server
    (bytevector-u8-set! bv 1 #x01)
    ;; jack_use_exact_name
    (bytevector-u8-set! bv 2 #x02)
    ;; jack_server_name
    (bytevector-u8-set! bv 3 #x04)
    ;; jack_load_name
    (bytevector-u8-set! bv 4 #x08)
    ;; jack_load_init
    (bytevector-u8-set! bv 5 #x10)
    ;; jack_session_id
    (bytevector-u8-set! bv 6 #x20)))                        
    

;; initialize a jack client
(define init_jack
  (jack_client_open "test-client" #x00 %null-pointer))

(init_jacK)

        
;; get the sample rate from jack server (pipewire)

;; initialize pd
(libpd_init)

;; receive messages from pd

;; send DSP 1 message to pd (note we still need to connect the jack client to
;; the pipewire graph either via GUI or config file)

;; load patch

;; register io ports

;; register audio process callbcak

;; go!
