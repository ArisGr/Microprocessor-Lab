#define F_CPU 16000000UL
#include "avr/io.h"
#include "util/delay.h"
int main(){
    
unsigned char duty;
unsigned char i;
unsigned char x, y;
unsigned char a[13] = {5,26,46,67,87,108,128,148,169,190,210,230,251};


TCCR1A = (1 << WGM10) | (1 << COM1A1);
TCCR1B = (1 << WGM12) | (1 << CS11);

DDRB |=  0b00111111;
DDRD |=  0b00000000;

duty = 128;
i = 6;
OCR1AL = duty;


while(1){
 
    x = ~PIND;
    
    if((x & 0x02)){
        
        if(i != 12){
            i +=1;
        }
        duty = a[i];
        OCR1AL = duty;
        while(1){
            y = PIND;
            OCR1AL = duty;
            if(y & 0x02) break;
        }
    }
    
    if((x & 0x04)){
        if(i != 0){
            i -=1;
        }
        duty = a[i];
        OCR1AL = duty; 
        while(1){
            y = PIND;
            OCR1AL = duty;
            if(y & 0x04) break;
        }
    }
    else {
         duty = a[i];
         OCR1AL = duty;
    }
    
    }
}


