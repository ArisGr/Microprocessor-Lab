#define F_CPU 16000000UL
#include "avr/io.h"
#include "util/delay.h"


unsigned char duty;
unsigned char i,a,p,z,q,d;
unsigned char x,y, gas, lights, camera;
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

int gas_detected(){
    lights = 1;
 
   _delay_ms(10);
        
        lcd_data('G');
      //  _delay_ms(2);
        lcd_data('A');
      //  _delay_ms(2);
        lcd_data('S');
       // _delay_ms(2);
        lcd_data(' ');
       // _delay_ms(2);
        lcd_data('D');
      //  _delay_ms(2);
        lcd_data('E');
        //_delay_ms(2);
        lcd_data('T');
        //_delay_ms(2);
        lcd_data('E');
       // _delay_ms(2);
        lcd_data('C');
       // _delay_ms(2);
        lcd_data('T');
        //_delay_ms(2);
        lcd_data('E');
       // _delay_ms(2);
        lcd_data('D');
        //_delay_ms(2);
        
        _delay_ms(20);
        return 0;
        
    
}

int clear(){
    lights = 0;
        _delay_ms(10);
        lcd_data('C');
      //  _delay_ms(2);
        lcd_data('L');
      //  _delay_ms(2);
        lcd_data('E');
      //  _delay_ms(2);
        lcd_data('A');
       // _delay_ms(2);
        lcd_data('R');
        
        _delay_ms(20);
        return 0;
        
    
}


int metatroph(){
   //lights = 0;
    z = ADCL;
    y = ADCH;
    
    z = z & 0b11000000;
   // a1 = a2 = a3 = 0;
    
    if(y >= 220){
        PORTB = 0b00100000;
    }
    else if(y >= 160){
        PORTB = 0b00010000; 
    }
    else if(y >= 80){
        PORTB = 0b00001000; 
    }
    else if(y >= 40){
        PORTB = 0b00000100; 
    }
    else if(y >= 20){
        PORTB = 0b00000010; 
    }
    else{
        PORTB = 0b00000001; 
    }
    
    
    if(y >= 51){
       
        gas_detected();
    }
    else{
        z = z >> 6;
        y = y << 2;
        y = y | z;
        if(y >= 204){
           
            gas_detected();
        }
        else{
           
            clear();
        }
    }
    
    
    return 0;
}

int main(){
    
DDRD |=  0b11111111;
DDRB |=  0b11111111;
DDRC |=  0b00000000;
    
ADMUX = 0b01100011;

ADCSRA = 0b10000111;






while(1){
    lcd_init();  
    
     
    
    
     ADCSRA |= (1 << ADSC);
    // _delay_ms(10);
    while(ADCSRA & 0b01000000){
   
    }
     metatroph();
    
     _delay_ms(10);
    
    if(lights == 1){
        if(camera == 1){
            camera = 0;
             PORTB = 0b00000000;
        }
        else{
            camera = 1;
        }
    }  
     _delay_ms(100);
     
    }
}




