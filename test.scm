#!Bin/metac

(define >= (lambda (a b) (or (= a b) (> a b))))
(define <= (lambda (a b) (or (= a b) (< a b))))
(define to-float (lambda (n) (+ n 0.0)))

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

(define numbers (lambda (a b)
		  (if (> a b)
		      []
		      (+ [a] (numbers (+ a 1) b)))))

(define filter-out (lambda (arr pred index)
		     (if (< index (size arr))
			 (do
			     (define value (at arr index))
			     (+ (if (pred value) [value] [])
				(filter-out arr pred (+ index 1))))
			 [])))

(define is-divisible (lambda (num div) (= (% num div) 0)))

(define primes (lambda (start end)
		 (if (< start end)
		     (filter-out
		      (primes (+ start 1) end)
		      (lambda (n) (or (>= start n) (not (is-divisible n start))))
		      0)
		     (numbers 2 end))))

(print "primes: "(primes 2 100) "\n")
