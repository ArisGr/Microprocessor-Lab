.include "m328PBdef.inc"
    
reset:
    ldi r24 , low(RAMEND)
    out SPL , r24
    ldi r24 , high(RAMEND)
    out SPH , r24
    
    ldi r24, low(500)
    ldi r25, high(500)
 main:
     rcall  wait_x_msec
     rjmp end
     

wait4:
    ret	    ;4 cycles
    
wait:
    nop     ;6 cycles
    nop
    nop
    nop
    nop
    nop
wait_x_msec: 
    ldi r26, 99   ;1 cycles
    
loop:   
    
    rcall wait4		;3+4 = 7 cycles
    dec r26		;1 cycle
    brne loop		;1 or 2 cycles
    sbiw r25 : r24 , 1	;2 cycles  
    brne wait		;1 or 2 cycles 
    

    ret	    ;4cycles
    
    
    
end:



