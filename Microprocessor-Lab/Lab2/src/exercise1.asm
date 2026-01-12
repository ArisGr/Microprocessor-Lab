.include "m328PBdef.inc"
.equ FOSC_MHZ=16		    ;
.equ DEL_ms=600
.equ Del_NU=FOSC_MHZ*DEL_ms
    
    
.org 0x0
clr r27
rjmp reset
.org 0x4
rjmp ISR1 
       

reset:
    
ldi r24,(1 << ISC11) | (1 << ISC10)	    ;Interrupt on rising edge of INT1
sts EICRA, r24

ldi r24, (1 << INT1)			    ;enable Interrupt INT1
out EIMSK, r24
sei					    ;enable interrupts
    
    
ldi r24, LOW(RAMEND)
out SPL, r24
ldi r24, HIGH(RAMEND)
out SPH, r24
    
clr r30				;PORTD as input
out DDRD, r30
ser r30				;PORTC as output
out DDRC, r30

main:
    rjmp main

delay_ms:			;delay routine used in ISR1

ldi r23, 249

loop_inn:

dec r23
nop
brne loop_inn
sbiw r24, 1
brne delay_ms
ret    
    
    
    
  
    
ISR1:					;INT1`subroutine
    push r24
    push r25
    in r24, SREG
    push r24
    
    ldi r24, (1 << INTF1)		;EIFR <- 1
    out EIFR, r24
    
    ldi r24, low(16*5)			;wait 5ms
    ldi r25, high(16*5)
    rcall delay_ms
    
    in r24, EIFR			;check if EIFR.1 = 0
    lsr r24				;if it is continue to subroutine
    lsr r24				;else go back to ISR1 
    brcs ISR1
    
    in r25, PIND			;check PIND
    com r25				;negative logic
    lsl r25				;if we press button PD7,print the same number and stop counting
    brcs done				
    subi r27, -1			;else increase r27 by one
    cpi r27, 32				;if it reaches 32,clear r27(r27 =0)
    brne done				;else continue and print it
    clr r27   
done:
    out PORTC, r27			;print r27
    pop r24
    out SREG, r24
    pop r25
    pop r24   
    reti
    








