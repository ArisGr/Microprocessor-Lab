.include "m328PBdef.inc"
.equ FOSC_MHZ=16		;operating freqency of microcontroller
.equ DEL_ms=600			;delay in ms(600 ms for this exercise)
.equ Del_NU=FOSC_MHZ*DEL_ms	;input for delay_ms routine
    
.org 0x0
rjmp reset
.org 0x2
rjmp ISR0

reset:

ldi r24,(1 << ISC01) | (1 << ISC00)	;interrupt on rising edge of INT0 pin
sts EICRA, r24

ldi r24, (1 << INT0)			;enable INT0 interrupt = PD2
out EIMSK, r24
sei					;enable interrupts

    
    
ldi r24, LOW(RAMEND)
out SPL, r24
ldi r24, HIGH(RAMEND)
out SPH, r24

    
clr r26					;PORTB as input
out DDRB, r26
ser r26					;PORTC as output
out DDRC, r26

loop1:
clr r26

loop2:
out PORTC, r26			;print number

ldi r24, low(DEL_NU)
ldi r25, high(DEL_NU)		;set delay(number of cycles)
rcall delay_ms
inc r26				;increase r26 and check if it is 32
cpi r26, 32
breq loop1			;if it is 32 clear it
rjmp loop2			;else move on to the next number

    
    

delay_ms:		;total delay of 1000*DEL_NU + 6 cycles

ldi r23, 249		;1 cycle

    

loop_inn:		;total delay(1 + 249*4 - 1 = 996 cycles) 

dec r23			;1 cycle
nop			;1 cycle
brne loop_inn		;1 or 2 cycles
sbiw r24, 1		;2 cycles
brne delay_ms		;1 or 2 cycles
ret			;4 cycles
    
    
    
ISR0:		    ;INT0 subroutine
push r26
push r24
push r25

   
    
ldi r27, 1	;register for interrupt output
clr r26		;register where we store number of buttons pressed
ldi r25, 7	;number of loops needed(7-1=6 to check all the buttons)
    
in r24, PINB	;Buttons pressed
com r24		;complementary due to negative logic

loop:
dec r25		  ;decrease number of loops left
cpi r25, 0	 
breq number	  ;after 6 loops we have the number in r26
lsr r24		  ;else rotate input register right and check carry
brcc loop	  ;if carry is 0 loop again
inc r26		  ;else if carry is 1 increase r26 as we found button pressed
rjmp loop	  ;and loop again

number:

cpi r26, 0	;if r26 = 0,we've pressed 0 buttons and we clear output register 
breq zero
cpi r26, 1	;if r26 = 1,1 button pressed and we print r27 which has initial value 1
breq done
lsl r27		;else we open one more led from lsb to msb by shifting left and
subi r27, -1	;adding 1(we move all the leds left once and open the LSB by 
dec r26		; adding 1)
rjmp number	;and loop again

    
zero:
    clr r27	
done:
out PORTC, r27	;r27 output
    

pop r25		;restore stack
pop r24
pop r26
    
    
    
push r25	;and wait extra 500 ms
push r24
in r24, SREG
push r24



ldi r24, low(16*500)
ldi r25, high(16*500)
rcall delay_ms
    

pop r24
out SREG, r24
pop r24
pop r25
reti









