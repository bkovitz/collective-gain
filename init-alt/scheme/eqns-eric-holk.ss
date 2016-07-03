
    ;;
    (define attempted-absorption 
     (lambda (x y)
      (matrix
       (Σ (in o organisms)
        (near-absorption (- x (o x:)) (- y (o y:)))))))

        (define giver? (λ (o) (< (random 1.0) (o g:))))

        (define-syntax =
        (syntax-rules (args)
         ((_ (nm (args fml* ...) b b* ...))
          (define nm
           (lambda (fml* ...)
             b b* ...)))
         ((_ (
