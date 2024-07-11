(import (chezscheme) (keywords))

(cp0-outer-unroll-limit 10)

(library-group
  (import (chezscheme) (ghi) (object) (matrix) (probability) (debug))

(define run
  (λ ()
    (random-seed (time-seed))
    (printf "sum of near-absorption: ~s\n"
          (let-sum ([delta near-absorption])
            (delta delta:)))
    (let ([world (object size: (object x: 200 y: 200)
                         num-gens: 10
                         acorn-distance-factor: 1.0)])
      (make-initial-population! world 20)
      (print-world world)
      (for ([gen (from-to 1 (world num-gens:))])
        (printf "~a\ngen ~d\n" (make-string 72 #\-) gen)
        (world 'set! gen: gen)
        (one-generation! world)
        (let* ([num-givers  (let-count ([o (world parents:)])
                              (o giver?:))]
               [num-attempted-offspring
                            (let-sum ([o (world parents:)])
                              (o num-attempted-offspring:))]
               [population  (length (world organisms:))]
               [num-missing-children
                            (- num-attempted-offspring population)]
               [missing-children-pct
                            (pct (/ num-missing-children
                                    num-attempted-offspring))])
          (printf "# givers: ~s\n" num-givers)
          (printf "attempted children: ~s\n" num-attempted-offspring)
          (printf "missing children: ~s (~s%)\n"
              num-missing-children missing-children-pct)
          (printf "new population: ~s\n" population))
        (print-world world)))))

(define pct
  (λ (x)
    (exact (round (* 100.0 x)))))

(define make-initial-population!
  (λ (world num-organisms)
    (world 'set! organisms: '())
    (repeat num-organisms
      (world 'cons! organisms: (object g: 0
                                       loc: (unoccupied-location world))))))

(define one-generation!
  (λ (world)
    (calculate-attempted-absorption! world)
    (printf "attempted absorption\n")
    (world attempted-absorption: 'print)
    (share-the-sunlight! world)
    (decide-who-becomes-a-giver! world)
    (givers-create-protection! world)
    (printf "protection\n")
    (world protection: 'print)
    (subtract-predation! world)
    (make-offspring! world)
    #;(print-organisms world parents:)))

(define distance
  (λ (loc1 loc2)
    (let ([dx (- (loc1 x:)
                 (loc2 x:))]
          [dy (- (loc1 y:)
                 (loc2 y:))])
      (sqrt (+ (* dx dx)
               (* dy dy))))))

(define make-radial-decay-deltas
  (λ (radius)
    (filter (λ (delta)
              (> (delta delta:) 0))
            (let-map ([y (from-to (- radius) radius)]
                      [x (from-to (- radius) radius)])
              (let ([delta (object x: x y: y)])
                (delta 'set! delta: (- radius
                                       (distance delta O)))
                delta)))))

(define near-absorption (make-radial-decay-deltas 4))
(define near-protection (make-radial-decay-deltas 4))

(define calculate-attempted-absorption!
  (λ (world)
    (world 'set! attempted-absorption: (zero-matrix size: (world size:)))
    (for ([o (world organisms:)])
      (apply-deltas! near-absorption
                     (world attempted-absorption:)
                     center: (o loc:)))))

(define share-the-sunlight!
  (λ (world)
    (for ([o (world organisms:)])
      (o 'set! sunlight-before-predation:
          (let-sum ([delta near-absorption])
            (share-of-sunlight-at world (o loc:) delta))))))

(define share-of-sunlight-at
  (λ (world loc delta)
    (let ([loc (loc+ loc delta)])
      (if (world attempted-absorption: 'in-bounds? loc)
          (if (> (world attempted-absorption: loc) 1.0)
              (/ (delta delta:)
                 (world attempted-absorption: loc))
              (world attempted-absorption: loc))
          0))))

(define decide-who-becomes-a-giver!
  (λ (world)
    (for ([o (world organisms:)])
      (o 'set! giver?: (<= (random 1.0)
                           (o g:))))))

(define givers-create-protection!
  (λ (world)
    (world 'set! protection: (zero-matrix size: (world size:)))
    (for ([o (world organisms:)])
      (when (o giver?:)
            (apply-deltas! near-protection
                           (world protection:)
                           center: (o loc:)
                           factor: (o sunlight-before-predation:))))))

(define subtract-predation!
  (λ (world)
    (for ([o (world organisms:)])
      (o 'set! sunlight: (- (o sunlight-before-predation:)
                            (* (o sunlight-before-predation:)
                               (expt 0.9
                                     (+ (world protection: (o loc:))
                                        1.0))))))))

(define make-offspring!
  (λ (world)
    (world 'set! parents: (world organisms:))
    (world 'set! organisms: '())
    (for ([p (world parents:)])
      (if (p giver?:)
          (p 'set! num-attempted-offspring: 0)
          (begin
            (p 'set! num-attempted-offspring: (random-poisson (p sunlight:)))
            (repeat (p num-attempted-offspring:)
              (make-one-child! world p)))))))

(define make-one-child!
  (λ (world p)
    (call/cc (λ (done)
      (repeat 20
        (let ([child-loc (random-location-centered-at
                              (p loc:)
                              (random-poisson (* (world acorn-distance-factor:)
                                                 (p sunlight:))))])
          (when (location-ok? world child-loc)
                (world 'cons! organisms:
                    (object loc: (object x: (child-loc x:) y: (child-loc y:))
                            g: (child-g p)))
                (done))))))))

(define child-g
  (λ (parent)
    (range-cap 0.0 1.0 (+ (parent g:) (g-mutation-delta)))))

(define g-mutation-delta (make-normal-generator 0.0 0.01))

(define random-location-centered-at
  (λ (loc dist)
    (let ([θ (random two-pi)])
      (loc+ loc (object x: (exact (round (* dist (cos θ))))
                        y: (exact (round (* dist (sin θ)))))))))

(define random-location
  (λ (size)
    (object x: (random (size x:))
            y: (random (size y:)))))

(define unoccupied-location
  (λ (world)
    (let loop ()
      (let ([loc (random-location (world size:))])
        (if (occupied? loc (world organisms:))
            (loop)
            loc)))))

(define location-ok?
  (λ (world loc)
    (and (>= (loc x:) 0)
         (>= (loc y:) 0)
         (<  (loc x:) (world size: x:))
         (<  (loc y:) (world size: y:))
         (not (occupied? loc (world organisms:))))))

(define occupied?
  (λ (loc ls)
    (exists (λ (o) (and (= (loc x:) (o loc: x:))
                        (= (loc y:) (o loc: y:))))
            ls)))

(define print-world
  (λ (world)
    (define grid (matrix size: (world size:) fill: "  -"))
    (for ([o (world organisms:)])
      (grid 'set! (o loc:) (format " ~2d" (exact (round (* 100.0 (o g:)))))))
    (grid 'print)))

(define print-organisms
  (λ (world key)
    (printf "organisms\n")
    (for ([o (world key)])
      (printf "g: ~s giver?: ~s loc: (~s,~s)\nsunlight-before-predation: ~s\nsunlight: ~s\nnum-attempted-offspring: ~s\n\n"
          (o g:)
          (o giver?:)
          (o loc: x:)
          (o loc: y:)
          (o sunlight-before-predation:)
          (o sunlight:)
          (o num-attempted-offspring:)))))

(collect)
(time (run))

;(import (tests))
;
;(test (begin (run) #t)
;      #t)

;(define one-generation!
;  (λ (world)
;    (calculate-attempted-absorption! world)
;    (share-the-sunlight! world)
;    (decide-who-becomes-a-giver! world)
;    (givers-create-protection! world)
;    (subtract-predation! world)
;    (make-offspring! world)))


)
