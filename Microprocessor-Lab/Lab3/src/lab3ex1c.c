#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


unsigned char check = 0;
unsigned char check2 = 0;



ISR(TIMER1_OVF_vect){
        PORTB = 0x00;
        sei();
}        

ISR(INT1_vect) {  
TCNT1 = 3036;
while(1){    
    EIFR = (1 << INTF0); 
    _delay_ms(5);
   // int a = EIFR &0x02;
    if(!(EIFR &0x01)) break;
}
check = 1;              //ENTERED INTERR
sei();

TCNT1 = 3036;
    if((PINB & 0x01)){  
        PORTB = 0b00111111;
        for(int i = 0; i < 500; i++){
            
            if(check == 0){
                break;
            }
            _delay_ms(1);
        }
    }    
        PORTB = 0x01;   //close leds
        check = 0;
        check2 = 0;
        
               //EIFR.1 = 0
       
}






int main()
{
    EICRA=(1 << ISC11) |(1 << ISC10);
    EIMSK= (1 << INT1);   
    
    TIMSK1 = ( 1 << TOIE1);

    TCCR1B =  (1<<CS12) | (0<<CS11) | (1<<CS10);
    
    DDRB |= 0xFF;            //PORTB output
    DDRD |= 0x00;            //PORTD input
    DDRC |= 0x00;
    
    while(1){    
        sei();              //enable interrupts
        check2 = 1;
        if( !(PINC & 0x20)){
            while(!(PINC & 0x20)){
                
            }
         
         _delay_ms(5);
            
             if((PINB & 0x01)){
                while( !(PINC & 0x20)){
                //PORTB = 0b00111111;
                _delay_ms(5);
                TCNT1 = 3036;
                }                
                PORTB = 0b00111111;
                for(int i = 0; i < 500; i++){
            
                    if(  ( !(PINC & 0x20)) || (check2 == 0) ){
                    break;
                    }
                    _delay_ms(1);
        }
                
    }   
            
            PORTB = 0x01;
            TCNT1 = 3036;
         
            
        }
        sei();
    } 
    sei();
}



