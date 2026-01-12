#define F_CPU 16000000UL
#include "avr/io.h"
#include "util/delay.h"


unsigned char duty;
unsigned char i,a,p,z,q,d;
unsigned char x,y;
unsigned char temp,a1,a2,a3;









void write_2_nibbles(unsigned char q){
    int temp = q;   // r24 = temp = q
    int e = PIND;   //in r25, PIND
    e = e & 0x0f;    // andi r25, 0x0f  
    temp = temp & 0xf0; // andi r24, 0xf0  
    temp = temp | e;    // add r24, r25
    PORTD= temp;  //out PORTD ,r24
    PORTD |= 0x08;  //sbi PORTD ,3
    _delay_ms(1);   
    PORTD &= 0b11110111; //cbi PORTD ,3
    _delay_ms(1);

    temp = q;   //pop r24
    temp = temp<<4;  //swap r24
    temp = temp & 0xf0;  //andi r24, 0xf0
    temp = temp | e;   //add r24,r25 
    PORTD= temp;   //portd, r24
    PORTD |= 0x08;
    _delay_ms(1);
    PORTD &= 0b11110111; 
    _delay_ms(2);

return;
} 


void lcd_data(unsigned char p){
    PORTD |= 0x04;
    write_2_nibbles(p);
    _delay_ms(10);
    return;
}

void lcd_command(unsigned char z){
    PORTD &= 0b11111011;
    write_2_nibbles(z);
    _delay_ms(10);
    return;
}


void lcd_init(){
        _delay_ms(40); //wait_msec

        PORTD = 0x30;     //PORTD = r24 = 0x30
        PORTD |= 0x08;   // sbi PORTD, 3 
        _delay_ms(1); 
        PORTD &= 0b11110111; //cbi PORTD, 3
        _delay_ms(2);

        PORTD = 0x30;   //PORTD = r24 = 0x30
        PORTD |= 0x08;
        _delay_ms(1); 
        PORTD &= 0b11110111;
        _delay_ms(2); 
        PORTD = 0x20;
        PORTD |= 0x08;
        _delay_ms(1);
        PORTD &= 0b11110111;
        _delay_ms(2); 
        lcd_command(0x28);
        lcd_command(0x0c);
        lcd_command(0x01);

        _delay_ms(10);

        lcd_command(0x06);
        
        _delay_ms(10);
        
        return;
} 

void metatroph(){
    z = ADCL;
    y = ADCH;   
    z = z & 0b11000000;
    a1 = a2 = a3 = 0;
    
    while(1){
        if(y >= 51){
            a1 += 1;
            y -= 51;
        }
        else break;
    }
   
    z = z >> 6;
    y = y << 2;
    y = y | z;
    
    while(1){
        if(y >= 204){
            a1 += 1;
            y -= 204;
        }
        else break;
    }
    
    while(1){
        if(y >= 20){
            a2 += 1;
            y -= 20;
        }
        else break;
    }
    
    while(1){
        if(y >= 2){
            a3 += 1;
            y -= 2;
        }
        else break;
    }
    
    a1 = a1 | 0b00110000;
    a2 = a2 | 0b00110000;
    a3 = a3 | 0b00110000;
    lcd_data(a1);
    lcd_data(',');
    lcd_data(a2);
    lcd_data(a3);
    _delay_ms(100);
    
    
    
    return;
}

int main(){
 DDRD |=  0b11111111;
DDRB |=  0b11111111;
DDRC |=  0b00000000;   

ADMUX = 0b01100000;

ADCSRA = 0b10000111;





while(1){
    lcd_init();  
    _delay_ms(2);
    

    
    ADCSRA |= (1 << ADSC);
    // _delay_ms(10);
    while(ADCSRA & 0b01000000){
   
    }
     metatroph();
     
     _delay_ms(10);
    
    
    }
}




