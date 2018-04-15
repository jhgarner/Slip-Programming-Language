(define if `(if1 (define (++ if $p1) $p2) (define (++ if (+ $p1 1)) $p3))')
(define for `(if $p1 `(_for (p6) $p5 $p6 $p4)' `(id $p4)' $p1 $p2 $p3)')
(define _for `(if (p2) `(_for (p6) $p5 $p6 $p4)' `(id $p7)' $p1 $p2 $p3 $p4)')
