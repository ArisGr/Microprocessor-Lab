.include "m328PBdef.inc"
.equ FOSC_MHZ=16
.equ DEL_ms=600
.equ Del_NU=FOSC_MHZ*DEL_ms
    
.org 0x0
rjmp reset
.org 0x4
rjmp ISR1
.org 0x1A
rjmp ISR_TIMER1_OVF
sei		    ;  timer1 

reset:
    
ldi r24,(1 << ISC11) | (1 << ISC10)	;enable interrupt on rising edge of INT1 
sts EICRA, r24

ldi r24, (1 << INT1)			;enable INT1 interrupt
out EIMSK, r24
    
ldi r24, (1<<TOIE1)			; ???????????? ???????? ???????????? ??? ??????? TCNT1
sts TIMSK1, r24				; ??? ??? timer1    
    
ldi r24 ,(1<<CS12) | (0<<CS11) | (1<<CS10) ; CK/1024
sts TCCR1B, r24    
    
clr r27
sei					;enable interrupts
    
   
ldi r24, LOW(RAMEND)
out SPL, r24
ldi r24, HIGH(RAMEND)
out SPH, r24
    
clr r30					;PORTD input and PORTb output
out DDRD, r30
out DDRC,r30
ser r30
out DDRB, r30

    
main:
    
    lsr r27		    ;if LSB of r27 is 0 then do nothing
    brcs start
    in r21,PINC
    lsl r21
    lsl r21
    lsl r21
    brcs again
    
 check: 
    in r21,PINC
    lsl r21
    lsl r21
    lsl r21
    brcc check

    
 start:   
    ldi r24, HIGH(3036) ; ???????????? ??? TCNT1
    sts TCNT1H, r24	; ??? ??????????? ???? ??? 3 sec
    ldi r24, LOW(3036)
    sts TCNT1L, r24
    
    
    in r25, PORTB		;store input in r25
    lsr r25			;check LSB of input
    ;lsr r27		    ;else check second LSB
    brcc no_refresh	    ;if it is 1 then we need to open all leds for 500ms
    
    
    ldi r28, 0xFF	    ;if refreshed,open all leds for 500ms
    
    out PORTB, r28	    ;r28 = 0xFF as output
    
    ldi r24, low(16*500)    ;500ms delay		
    ldi r25, high(16*500)
    rcall delay_ms
    
no_refresh:   
    ldi r28, 0x01	    ;else we need to open LSB led of portB for 4s
    out PORTB, r28	    ;r28 = 1 as output
       
    
      again:
      ;in r25, PORTB
     ; ldi r27, 0x03
     ; lsr r25			;check LSB of input
      brcs main
      clr r27
      rjmp main
    

      
      
      
delay_ms:		;delay routine used in previous exercises too

ldi r23, 249

loop_inn:

dec r23
nop
brne loop_inn
sbiw r24, 1
brne delay_ms
ret    
 
    
ISR1:
     
    push r24
    push r25
    in r24, SREG
    push r24
    
    ldi r24, (1 << INTF1)	    ;qweqweqweqweqweqwe
    out EIFR, r24		    
    
    ldi r24, low(16*5)		;start spin8
    ldi r25, high(16*5)
    rcall delay_ms
    
    in r24, EIFR
    lsr r24
    lsr r24
    brcs ISR1			;end spin8
    
    ldi r24, HIGH(3036) ; ???????????? ??? TCNT1
    sts TCNT1H, r24	; ??? ??????????? ???? ??? 3 sec
    ldi r24, LOW(3036)
    sts TCNT1L, r24
    
    
    ldi r27, 0x01		;r27 = 00000001
    in r25, PORTB		;store input in r25
    lsr r25			;check LSB of input
    brcc done			;if MSB LED is closed then continue to main
    ldi r27, 0x03		;else r27 = 00000011 and continue to main
    
     
done: 
    pop r24
    out SREG, r24
    pop r25
    pop r24
    sei	    
    rjmp main
    


    
ISR_TIMER1_OVF:
    push r24
    push r25
    in r24, SREG
    push r24
    
    ldi r24, 0x00
    out PORTB, r24
    
    pop r24
    out SREG, r24
    pop r25
    pop r24
    reti
    
    

    
    
   








