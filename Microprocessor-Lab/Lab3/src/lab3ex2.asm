.include "m328PBdef.inc"

.org 0x00    
reset:
    
ldi r24 , (1<<WGM10) | (1<<COM1A1);COM1A overrides PORTx
sts TCCR1A, r24
    
ldi r24 ,(1<<WGM12) | (1 << CS11) 
sts TCCR1B, r24
    
  
 ldi r24, 0x80		;DUty cycle 50%,TOP = 0x80
 sts OCR1AL, r24        ;    Bottom = 0
 clr r24  
 sts OCR1AH, r24 
 
 ldi r24, LOW(RAMEND)
out SPL, r24
ldi r24, HIGH(RAMEND)
out SPH, r24

ser r24
out DDRB, r24    
 
 
ldi r29, 0x0C 
 
clr r24
out DDRD, r24
 
 ldi Zh, high(Table*2)
 ldi Zl, low(Table*2+12)
    
 clr r18
   ; add Zl, r29
   ; adc Zh, r18
    lpm
    mov r20, r0
   ; ldi r20, 128
    sts OCR1AL, r20        ;    Bottom = 0
    clr r20  
    sts OCR1AH, r20
    
 
main:
     ldi Zh, high(Table * 2)
     ldi Zl, low(Table * 2 )
     
   
    
    in r24, PIND
    com r24
    cpi r24, 0x02
    breq increase
    cpi r24, 0x04
    breq decrease
    rjmp main
    
increase:
    
    checki: 
    ldi r24, low(16*5)    ;500ms delay		
    ldi r25, high(16*5)
    rcall delay_ms
    in r24, PIND
    com r24
    cpi r24, 0x02
    breq checki
    
    cpi r29, 24
    breq no_increase
    subi r29, -2
no_increase:
    clr r18
    add Zl, r29
    adc Zh, r18
    lpm
    mov r20, r0
    
    
    sts OCR1AL, r20        ;    Bottom = 0
    clr r20  
    sts OCR1AH, r20
    
    rjmp main
decrease:
    
    checkd: 
    ldi r24, low(16*5)    ;500ms delay		
    ldi r25, high(16*5)
    rcall delay_ms
    in r24, PIND
    com r24
    cpi r24, 0x04
    breq checkd
    
     cpi r29, 0
    breq no_decrease
    subi r29, 2
no_decrease:   
    clr r18
    add Zl, r29
    adc Zh, r18
    lpm
    mov r20, r0
    
    sts OCR1AL, r20        ;    Bottom = 0
    clr r20  
    sts OCR1AH, r20
    
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
 
    
.org 0x2000 
Table:
.DW  0x0006, 0x001A,0x002E, 0x0043 ,0x0057 ,0x006C,0x0080, 0x0094 , 0x00A9 , 0x00BD , 0x00D2 , 0x00E6 ,0x00FA

;.DW  0x06, 0x1A,0x2E, 0x43 ,0x57 ,0x6C,0x80, 0x94 , 0xA9 , 0xBD , 0xD2 , 0xE6 ,0xFA
;.DW  0x1A06, 0x432E,0x6C57, 0x9480 ,0xBDA9 ,0xE6D2,0x00FA

    
   

    
 
    
    
    










