#define F_CPU 16000000UL
#include "avr/io.h"
#include "util/delay.h"


unsigned char duty;
unsigned char i,a,p,z,q,d,b;
unsigned char x,y;
unsigned char temp,a1,a2,a3;

void write_2_nibbles(unsigned char q){
    int temp = q;   // r24 = temp = q
    int e = PIND;   //in r25, PIND
    e = e & 0x0f;    // andi r25, 0x0f  
    temp = temp & 0xf0; // andi r24, 0xf0  
    temp = temp + e;    // add r24, r25
    PORTD= temp;  //out PORTD ,r24
    PORTD |= 0x08;  //sbi PORTD ,3
    _delay_ms(1);   
    PORTD &= 0b11110111; //cbi PORTD ,3
    _delay_ms(1);

    temp = q;   //pop r24
    temp = temp<<4;  //swap r24
    temp = temp & 0xf0;  //andi r24, 0xf0
    temp = temp + e;   //add r24,r25 
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


void metatroph(unsigned char d){
    z = ADCL;
    y = ADCH;   
    z = z & 0b11000000;
    a1 = a2 = a3 = 0;
    lcd_init();
    
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
        if(y >= 200){
            a1 += 1;
            y -= 200;
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
    
    //b = a1*100 +a2*10+ a3*1;
    
            
           
    
    d = d | 0b00110000;
    lcd_data(d);
    lcd_data('0');
    lcd_data('%');
    
    lcd_command(0b11000000);
    
    a1 = a1 | 0b00110000;
    a2 = a2 | 0b00110000;
    a3 = a3 | 0b00110000;
    lcd_data(a1);
    lcd_data(',');
    lcd_data(a2);
    lcd_data(a3);
    _delay_ms(500);
    
    
    
    return;
}

int main(){
    



TCCR1A = (1 << WGM11) | (0 << WGM10)| (1 << COM1A1);
TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11) |(1<< CS10);


ADMUX =0b01100001;


ADCSRA =0b10000111;


DDRD |=  0b11111111;
DDRB |=  0b00000010;
DDRC |=  0b00000000;


duty = 128;
OCR1AL = duty;
PORTD = 0;
lcd_init();
lcd_init();
lcd_init();

while(1){
    lcd_init();  
    _delay_ms(2);
    
    x = ~PINB;
    ICR1 = 0; 
   // TCCR1A = (0 << WGM11) | (0 << WGM10)| (0 << COM1A1);
    
    if( x & 0x04){
        TCCR1A = (1 << WGM11) | (0 << WGM10)| (1 << COM1A1);
        TCNT1=0;             /////
        
       while( x & 0x04 ){        //////
        
        
       OCR1AL = 10;
       ICR1 = 50;
       a = 0b00110010;
       
       ADCSRA |= (1 << ADSC);
    // _delay_ms(10);
    while(ADCSRA & 0b01000000){
   
    }
     metatroph(a);
    
     _delay_ms(10);
     
     
     x = ~PINB;
       }               ////
       
       }
    
    
    else if( x & 0x08 ){
        OCR1AL = 20;
        ICR1 = 50;
        a = 0b00110100;
        
        ADCSRA |= (1 << ADSC);
    // _delay_ms(10);
    while(ADCSRA & 0b01000000){
   
    }
     metatroph(a);
    
     _delay_ms(10);
        
    }
    
    else if( x & 0x10 ){
       OCR1AL = 30;
       ICR1 = 50;
       a = 0b00110110;
       
       ADCSRA |= (1 << ADSC);
    // _delay_ms(10);
    while(ADCSRA & 0b01000000){
   
    }
     metatroph(a);
    
     _delay_ms(10);
        
    }
    
    else if( x & 0x20 ){
           TCCR1A = (1 << WGM11) | (0 << WGM10)| (1 << COM1A1);
        TCNT1=0;             /////
        
       while( x & 0x20 ){        //////
        
       OCR1AL = 40;
       ICR1 = 50;
       a = 0b00111000;
       
       ADCSRA |= (1 << ADSC);
    // _delay_ms(10);
    while(ADCSRA & 0b01000000){
   
    }
     metatroph(a);
    
     _delay_ms(10);
         x = ~PINB;
       }               ////
        
    }
    
    else{
      //  TCCR1A = (0 << WGM11) | (0 << WGM10)| (0 << COM1A1);
    OCR1AL = 0;
    ICR1 = 0;
    a = 0b00110000;
    
    ADCSRA |= (1 << ADSC);
    // _delay_ms(10);
    while(ADCSRA & 0b01000000){
   
    }
     metatroph(a);
    
     _delay_ms(10);
   
     
    }
    
     /*ADCSRA |= (1 << ADSC);
    // _delay_ms(10);
    while(ADCSRA & 0b01000000){
   
    }
     metatroph(a);
    
     _delay_ms(10);
    */
    
    }
}


