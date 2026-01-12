
.include "m328PBdef.inc" 

reset:
	ldi r24 , low(RAMEND)  
	out SPL , r24
	ldi r24 , high(RAMEND)
	out SPH , r24
	ser r24
	out DDRD , r24			
	ldi r27 , 1			
	SET 
	rjmp main

now_left:
	lsl r27
	SET
	ldi r24 , low(1000)		 
	ldi r25 , high(1000)		
	rcall wait_x_msec

main:
	out PORTD , r27
	ldi r24 , low(500)		
	ldi r25 , high(500)		
	rcall wait_x_msec
	BRTC move_right 
	
left:
	cpi r27 , 0x80
	breq right
	lsl r27
	rjmp main
right:
	CLT
	ldi r24 , low(1000)		
	ldi r25 , high(1000)		
	rcall wait_x_msec

move_right:	
	cpi r27 , 1				
	breq now_left
	lsr r27
	rjmp main
	
	
	;;;;;;;;;;;;;
wait4:
    ret
    
wait:
    nop
    nop
    nop
    nop
    nop
    nop
wait_x_msec:
    
    ldi r26, 99
    
loop:
    rcall wait4
    dec r26
    brne loop
    sbiw r25 : r24 , 1
    brne wait
   
    ret

