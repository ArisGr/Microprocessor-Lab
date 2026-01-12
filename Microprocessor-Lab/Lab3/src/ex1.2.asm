
.include "m328PBdef.inc"
 
    
.DEF A=r16 ; ?????? ???????????
.DEF B=r17 ; ??????????
.DEF C=r18
.DEF D=r19
.DEF F0=r20    
.DEF F1=r21   
.DEF temp=r22
.DEF var = r23
    
ldi A, 0x55
ldi B, 0x43
ldi C, 0x22    
ldi D, 0x02
ldi var, 0x06
    
make:
    mov F0, A
    or F0, B
    mov temp, D
    com temp
    or temp, B
    and F0, temp
    
    mov F1, A
    or F1, C
    and F1, temp
    
    
subi A, -2
subi B, -3
subi C, -4    
subi D, -5    
dec var
brne make    

end:
    
    
    





