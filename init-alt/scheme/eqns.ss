(simulation

  (range x 1 WORLD-X)
  (range y 1 WORLD-Y)

  (range dx (- ABSORPTION-RADIUS) ABSORPTION-RADIUS)
  (range dy (- ABSORPTION-RADIUS) ABSORPTION-RADIUS)

  (= (near-absorption (args dx dy))
          (delta-matrix
            (max 0.0
                 (- ABSORPTION-RADIUS
                    (* (distance-from-origin dx dy) ABSORPTION-RADIUS)))))

  (= (distance-from-origin (args x y))
          (function
            (sqrt (+ (expt x 2)
                     (expt y 2)))))

  (each-time-step
  
    (persistent organisms
                (set (object x: y: g:)))

    (= (attempted-absorption (args x y))
       (matrix
         (Σ (in o organisms)
            (near-absorption (- x (o x:)) (- y (o y:))))))

    (= (attempted-absorption x y)
       (matrix
         (Σ ([o organisms])
            (near-absorption (- x (o x:)) (- y (o y:))))))

    (= (o giver?:)
       (< (random 1.0) (o g:)))

    WRONG: sunlight is an attribute of an organism
    (= (sunlight (args x y))
       (matrix
         (Σ (for x y)
            (cond
              [(<= (attempted-absorption x y) 1.0)
                (near-absorption (- x (o x:)) (- y (o y:)))]
              [(> (attempted-absorption x y) 1.0)
                (/ (near-absorption (- x (o x:)) (- y (o y:)))
                   (attempted-absorption x y))]))))

    (= givers
       (set (in o organisms)
            (such-that (o giver?:))))

    (= (protection (args x y))
       (matrix
         (Σ (in o givers)
              (
  )
)

