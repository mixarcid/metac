#!Bin/metac

(define >= (lambda (a b) (or (= a b) (> a b))))
(define <= (lambda (a b) (or (= a b) (< a b))))

(define dotimes (lambda (a b f)
		  (if (>= a b)
		      (f a)
		      (do
			  (f a)
			  (dotimes (+ a 1) b f)))))

(define fac (lambda (n)
	      (if (< n 2)
		  1
		  (* n (fac (- n 1))))))

(define fib (lambda (n)
	      (if (< n 2)
		  1
		  (+ (fib (- n 1)) (fib (- n 2))))))

#this is a comment

(define arr [])
(define arr2 [])
(define arr3 [])
(dotimes 0 10 (lambda (n)
		(do
		  (define arr (+ arr [(fib n)]))
		  (define arr2 (+ arr2 [(fac n)]))
		  (define arr3 (+ arr3 [n])))))

(print "fibs: " arr "\n"
       "facs: " arr2 "\n"
       "numbers: " arr3 "\n")
