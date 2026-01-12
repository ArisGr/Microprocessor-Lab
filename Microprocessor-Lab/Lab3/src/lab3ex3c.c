#define F_CPU 16000000UL
#include "avr/io.h"
#include "util/delay.h"

int main(){
    
unsigned char duty;
//unsigned char i;
unsigned char x;


TCCR1A = (1 << WGM11) | (0 << WGM10)| (1 << COM1A1);
TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS12) |(1<< CS10);

DDRB |=  0b00111111;
DDRD |=  0b00000000;

duty = 128;
OCR1AL = duty;


while(1){
    x = ~PIND;
    
    if( x & 0x01){
        TCNT1=0;
        
       while( x & 0x01 ){
       OCR1AL = 62;
       ICR1 = 124; 
       x = ~PIND;
       }
    
    }
    
    
    else if( x & 0x02 ){
        TCNT1=0;
        
       while( x & 0x02 ){
        OCR1AL = 31;
        ICR1 = 62; 
        x = ~PIND;
       }
        
    }
    
    else if( x & 0x04 ){
        TCNT1=0;
        
       while( x & 0x04 ){
       OCR1AL = 15;
        ICR1 = 31; 
       x = ~PIND;
       }
    }
    
    else if( x & 0x08 ){
        TCNT1=0;
        
       while( x & 0x08 ){
       OCR1AL = 7;
        ICR1 = 15; 
        x = ~PIND;
       }
    }
    
    else{
    OCR1AL = 0;
    ICR1 = 0;
    TCNT1 = 0;
    }
    }
}

