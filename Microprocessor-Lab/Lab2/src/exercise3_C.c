#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

ISR(INT1_vect)
{    
for(int i = 0; i < 4000; i++){                  //for checking every 1 ms if we have new interrupt
        if(PIND & 0x08 == 1){                   //if new interrupt is detected
            for(int j = 0; j < 500; j++){       //open all LEDS for 500 ms while checking for new interrupts
            if(PIND & 0x08 == 1) j=0;           //for every new interrupt we detect, j = 0 (we print all leds for 500 ms again)
            PORTB = 0xFF;
            _delay_ms(1);
           }
            PORTB = 0x01;                       //after all leds go out
            i = 499;                            //i = 499 and we have 3500 more loops to print LSB LED for 3500 ms
            continue;
        }
        else {
            PORTB = 0x01;                       //print LSB
            _delay_ms(1);
         }
    }
    PORTB = 0x00;               //close leds
    EIFR = (1 << INTF0);        //EIFR.1 = 0
    
}


int main()
{
    EICRA=(1 << ISC11) |(1 << ISC10);
    EIMSK= (1 << INT1);   
    
    DDRB = 0xFF;            //PORTB output
    DDRD = 0x00;            //PORTD input
    
    while(1){    
        sei();              //enable interrupts
    } 
}

