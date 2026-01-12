.include "m328PBdef.inc"

    
reset:
    
ldi r24 ,(1<<WGM11) | (0<<WGM10) | (1<<COM1A1)  ;COM1A overrides PORTx
sts TCCR1A, r24
    
ldi r24 ,(1<<WGM12) | (1<<WGM13) | (1<< CS12) |(1<< CS10)
sts TCCR1B, r24
    
  
 ldi r24, 0x80		;DUty cycle 50%,TOP = 0x80
 sts OCR1AL, r24        ;    Bottom = 0
 clr r24  
 sts OCR1AH, r24 
 		;DUty cycle 50%,TOP = 0x80
 sts ICR1L, r24        ;    Bottom = 0
 sts ICR1H, r24  
    
clr r24
out DDRD, r24
ser r24
out DDRB,r24
    
    
main:
    ldi r24, LOW(RAMEND)
    out SPL, r24
    ldi r24, HIGH(RAMEND)
    out SPH, r24
    
    
  ;  ldi r24, 0x02
  ;  out DDRB, r24
   ldi r24, HIGH(0) ; ???????????? ??? TCNT1
    sts TCNT1H, r24	; ??? ??????????? ???? ??? 3 sec
    ldi r24, LOW(0)
    sts TCNT1L, r24
    
    in r24, PIND
    com r24
    cpi r24, 0x01
    breq Button0
    cpi r24, 0x02
    breq Button1
    cpi r24, 0x04
    breq Button2
    cpi r24, 0x08
    breq Button3
    rjmp main
    
Button0:
    
 ldi r24, 62		;DUty cycle 50%
 sts OCR1AL, r24          
 clr r24  
 sts OCR1AH, r24 
 ldi r24, 124		;TOP
 sts ICR1L, r24       
 clr r24
 sts ICR1H, r24 

 in r24, PIND
 com r24
 cpi r24, 0x01
 breq Button0
 rjmp exit
Button1:
  ldi r24, 31		;DUty cycle 50%
 sts OCR1AL, r24          
 clr r24  
 sts OCR1AH, r24 
 ldi r24, 62		;TOP
 sts ICR1L, r24       
 clr r24
 sts ICR1H, r24 
 
 in r24, PIND
 com r24
 cpi r24, 0x02
 breq Button1
 rjmp exit
    rjmp exit
    
    
Button2:
 ldi r24, 16		;DUty cycle 50%
 sts OCR1AL, r24          
 clr r24  
 sts OCR1AH, r24 
 ldi r24, 31		;TOP
 sts ICR1L, r24       
 clr r24
 sts ICR1H, r24 
 
 in r24, PIND
 com r24
 cpi r24, 0x04
 breq Button2
 rjmp exit
    rjmp exit
Button3:
     ldi r24, 8		;DUty cycle 50%
 sts OCR1AL, r24          
 clr r24  
 sts OCR1AH, r24 
 ldi r24, 15		;TOP
 sts ICR1L, r24       
 clr r24
 sts ICR1H, r24

 in r24, PIND
 com r24
 cpi r24, 0x08
 breq Button3
 rjmp exit
   
exit:
  
    rjmp main
    
    
    
    
    
    

    


    
    
    
    
  








