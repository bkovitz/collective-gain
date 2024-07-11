(import (ghi) (object))

(define calculate-attempted-absorption!
  (λ (world)
    (world 'set! attempted-absorption: (zero-matrix (world 'xy)))
    (for ([o (world organisms:)])
      (apply-delta-matrix! near-absorption
                           (o 'xy)
                           1.0
                           (world 'attempted-absorption)))

(define make-initial-population!
  (λ (world num-organisms)
    (world 'set! 'organisms
      (let loop ([i 0] [organisms '()])
        (cond
          [(>= i num-organisms)
            organisms]
          [else
            (loop (add1 i)
                  (: 'g 0
                     'xy (any-unoccupied-location (world 'xy) organisms)))])))))

(define any-unoccupied-location
  (λ (xy-size organisms)
    (let loop ([x (random (xy-size 'x))] [y (random (xy-size 'y))])
      (

(define run
  (λ ()
    (let ([world (: 'xy (: 'x 20 'y 20)
                    'num-gens 10)])
      (make-initial-population! world 10)
      (print-organisms world))))

;(define one-generation!
;  (λ (world)
;    (calculate-attempted-absorption! world)
;    (share-the-sunlight! world)
;    (decide-who-becomes-a-giver! world)
;    (givers-create-protection! world)
;    (subtract-predation! world)
;    (make-offspring! world)))
