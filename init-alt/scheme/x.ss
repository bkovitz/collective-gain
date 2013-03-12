(import (ghi) (object))

(define o (object))

(o 'set! 'a 1)

(printf "~s\n" (o 'a))



(define one-generation!
  (λ (world)
    ; attempted absorption
    (world 'set! 'attempted-absorption (zero-matrix (world 'xy)))
    (for ([o (world 'organisms)])
      (apply-delta-matrix! near-absorption
                           (o 'xy)
                           1.0
                           (world 'attempted-absorption)))
                           
    ; share the sunlight
    (for ([o (world 'organisms)])
      (o 'set! 'sunlight-before-predation
          (let-sum ([xy-delta (near-absorption 'xy-deltas)])
            (/ (near-absorption xy-delta)
               ((world 'attempted-absorption) (xy+ (o 'xy) xy-delta))))))

    ; decide who becomes a giver
    (for ([o (world 'organisms)])
      (o 'set! 'giver? (<= (uniform-random 0.0 0.1)
                           (o 'g))))

    ; givers create protection
    (world 'set! 'protection
        (fold-left! (zero-matrix (world 'xy))
                    (λ (protection-matrix o)
                      (apply-delta-matrix! near-absorption
                                           (o 'xy)
                                           (o 'sunlight-before-predation)
                                           protection-matrix))
                    (let-filter ([o (world 'organisms)])
                      (o 'giver?))))

    ; predation
    (for ([o (world 'organisms)])
      (o 'set! 'sunlight (- (o 'sunlight-before-predation)
                            (* (o 'sunlight-before-predation)
                               (expt 0.9
                                     (+ ((world 'protection-matrix) (o 'xy))
                                        1.0))))))

    ; make offspring
    (world 'set! 'organisms (fold-left! '()
                                        (λ (new-os o)
                                          (make-offspring! new-os o))
                                        (world 'organisms)))))
                                    
