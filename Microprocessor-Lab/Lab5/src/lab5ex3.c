#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define PCA9555_0_ADDRESS 0x40 //A0=A1=A2=0 by hardware
#define TWI_READ 1 // reading from twi device
#define TWI_WRITE 0 // writing to twi device
#define SCL_CLOCK 100000L // twi clock in Hz
//Fscl=Fcpu/(16+2*TWBR0_VALUE*PRESCALER_VALUE)
#define TWBR0_VALUE ((F_CPU/SCL_CLOCK)-16)/2
// PCA9555 REGISTERS
typedef enum {
 REG_INPUT_0 = 0,
 REG_INPUT_1 = 1,
 REG_OUTPUT_0 = 2,
 REG_OUTPUT_1 = 3,
 REG_POLARITY_INV_0 = 4,
 REG_POLARITY_INV_1 = 5,
 REG_CONFIGURATION_0 = 6,
 REG_CONFIGURATION_1 = 7,
} PCA9555_REGISTERS;
//----------- Master Transmitter/Receiver -------------------
#define TW_START 0x08
#define TW_REP_START 0x10
//---------------- Master Transmitter ----------------------
#define TW_MT_SLA_ACK 0x18
#define TW_MT_SLA_NACK 0x20
#define TW_MT_DATA_ACK 0x28
//---------------- Master Receiver ----------------
#define TW_MR_SLA_ACK 0x40
#define TW_MR_SLA_NACK 0x48
#define TW_MR_DATA_NACK 0x58

#define TW_STATUS_MASK 0b11111000
#define TW_STATUS (TWSR0 & TW_STATUS_MASK)
//initialize TWI clock

unsigned char A,B,C,D,temp1,temp2,x,F1,F0;




void twi_init(void)
{
 TWSR0 = 0; // PRESCALER_VALUE=1
 TWBR0 = TWBR0_VALUE; // SCL_CLOCK 100KHz
}
// Read one byte from the twi device ( request more data from device)
unsigned char twi_readAck(void)
{
TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
while(!(TWCR0 & (1<<TWINT))); 
 return TWDR0;
}

unsigned char twi_readNak(void)
{
        TWCR0 = (1<<TWINT) | (1<<TWEN);
        while(!(TWCR0 & (1<<TWINT)));

    return TWDR0;
}

// Issues a start condition and sends address and transfer direction.
// return 0 = device accessible, 1= failed to access device
unsigned char twi_start(unsigned char address)
{
 uint8_t twi_status;
// send START condition
TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
// wait until transmission completed
while(!(TWCR0 & (1<<TWINT)));
// check value of TWI Status Register.
twi_status = TW_STATUS & 0xF8;
if ( (twi_status != TW_START) && (twi_status != TW_REP_START)) return 1;
// send device address
TWDR0 = address;
TWCR0 = (1<<TWINT) | (1<<TWEN);
// wail until transmission completed and ACK/NACK has been received
while(!(TWCR0 & (1<<TWINT)));
// check value of TWI Status Register.
twi_status = TW_STATUS & 0xF8;
if ( (twi_status != TW_MT_SLA_ACK) && (twi_status != TW_MR_SLA_ACK) )
 {
 return 1;
 }
return 0;
}

// Send start condition, address, transfer direction.
// Use ack polling to wait until device is ready
void twi_start_wait(unsigned char address)
{
 uint8_t twi_status;
 while ( 1 )
 {
// send START condition
TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
 
 // wait until transmission completed
 while(!(TWCR0 & (1<<TWINT)));
 
 // check value of TWI Status Register.
 twi_status = TW_STATUS & 0xF8;
 if ( (twi_status != TW_START) && (twi_status != TW_REP_START)) continue;
 
 // send device address
 TWDR0 = address;
 TWCR0 = (1<<TWINT) | (1<<TWEN);
 
 // wail until transmission completed
 while(!(TWCR0 & (1<<TWINT)));
 
 // check value of TWI Status Register.
 twi_status = TW_STATUS & 0xF8;
 if ( (twi_status == TW_MT_SLA_NACK )||(twi_status ==TW_MR_DATA_NACK) ) 
 { 
 /* device busy, send stop condition to terminate write operation */
 TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
 
 // wait until stop condition is executed and bus released
 while(TWCR0 & (1<<TWSTO));
 
 continue;
 }
break;
 }
}
// Send one byte to twi device, Return 0 if write successful or 1 if write failed
unsigned char twi_write( unsigned char data )
{
// send data to the previously addressed device
TWDR0 = data;
TWCR0 = (1<<TWINT) | (1<<TWEN);
// wait until transmission completed

while(!(TWCR0 & (1<<TWINT)));
if( (TW_STATUS & 0xF8) != TW_MT_DATA_ACK) return 1;
return 0;
}
// Send repeated start condition, address, transfer direction 
//Return: 0 device accessible
// 1 failed to access device
unsigned char twi_rep_start(unsigned char address)
{
 return twi_start( address );
}
// Terminates the data transfer and releases the twi bus
void twi_stop(void)
{
 // send stop condition
TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
// wait until stop condition is executed and bus released
while(TWCR0 & (1<<TWSTO));
}
void PCA9555_0_write(PCA9555_REGISTERS reg, uint8_t value)
{
 twi_start_wait(PCA9555_0_ADDRESS + TWI_WRITE);
 twi_write(reg);
 twi_write(value);
 twi_stop();
}
uint8_t PCA9555_0_read(PCA9555_REGISTERS reg)
{
uint8_t ret_val;
 
 twi_start_wait(PCA9555_0_ADDRESS + TWI_WRITE);
 twi_write(reg);
 twi_rep_start(PCA9555_0_ADDRESS + TWI_READ);
 ret_val = twi_readNak();
 twi_stop();
 
 return ret_val;
}
void write_2_nibbles(unsigned char q){
    int temp = q;   // r24 = temp = q
    int e = PCA9555_0_read(REG_OUTPUT_0);   //in r25, PIND
    e = e & 0x0f;    // andi r25, 0x0f  
    temp = temp & 0xf0; // andi r24, 0xf0  
    temp = temp + e;    // add r24, r25
      //out PORTD ,r24
    PCA9555_0_write(REG_OUTPUT_0,temp);  //out PORTD ,r24
    temp |= 0x08;  //sbi PORTD ,3
    PCA9555_0_write(REG_OUTPUT_0,temp);
    _delay_ms(1);   
    temp &= 0b11110111; //cbi PORTD ,3
    PCA9555_0_write(REG_OUTPUT_0,temp);
    _delay_ms(1);

    temp = q;   //pop r24
    temp = temp<<4;  //swap r24
    temp = temp & 0xf0;  //andi r24, 0xf0
    temp = temp + e;   //add r24,r25 
    PCA9555_0_write(REG_OUTPUT_0,temp);   //portd, r24
    temp |= 0x08;
    PCA9555_0_write(REG_OUTPUT_0,temp);
    _delay_ms(1);
    temp &= 0b11110111; 
    PCA9555_0_write(REG_OUTPUT_0,temp);
    _delay_ms(2);

return;
} 


void lcd_data(unsigned char p){
    int temp = PCA9555_0_read(REG_OUTPUT_0);
    temp |= 0x04;
    PCA9555_0_write(REG_OUTPUT_0,temp);
    write_2_nibbles(p);
    _delay_ms(1);
    return;
}

void lcd_command(unsigned char z){
    unsigned char temp = PCA9555_0_read(REG_OUTPUT_0);
    temp &= 0b11111011;
    PCA9555_0_write(REG_OUTPUT_0,temp);
    write_2_nibbles(z);
    _delay_ms(1);
    return;
}


void lcd_init(){
    unsigned char temp;
    _delay_ms(40); //wait_msec
        
        PCA9555_0_write(REG_OUTPUT_0,0x30);   //PORTD = r24 = 0x30
        temp = PCA9555_0_read(REG_OUTPUT_0);
        temp |= 0x08;   // sbi PORTD, 3 
        PCA9555_0_write(REG_OUTPUT_0,temp);
        _delay_ms(1); 
        temp = PCA9555_0_read(REG_OUTPUT_0);
        temp &= 0b11110111; //cbi PORTD, 3
        PCA9555_0_write(REG_OUTPUT_0,temp);
        _delay_ms(2);

         PCA9555_0_write(REG_OUTPUT_0,0x30);   //PORTD = r24 = 0x30
        temp = PCA9555_0_read(REG_OUTPUT_0);
        temp |= 0x08;   // sbi PORTD, 3 
        PCA9555_0_write(REG_OUTPUT_0,temp);
        _delay_ms(1); 
        temp = PCA9555_0_read(REG_OUTPUT_0);
        temp &= 0b11110111; //cbi PORTD, 3
        PCA9555_0_write(REG_OUTPUT_0,temp);
        _delay_ms(2);
        
        PCA9555_0_write(REG_OUTPUT_0,0x30);   //PORTD = r24 = 0x30
        temp = PCA9555_0_read(REG_OUTPUT_0);
        temp |= 0x08;   // sbi PORTD, 3 
        PCA9555_0_write(REG_OUTPUT_0,temp);
        _delay_ms(1); 
        temp = PCA9555_0_read(REG_OUTPUT_0);
        temp &= 0b11110111; //cbi PORTD, 3
        PCA9555_0_write(REG_OUTPUT_0,temp);
        _delay_ms(2);
        
      PCA9555_0_write(REG_OUTPUT_0,0x20);   //PORTD = r24 = 0x30
        temp = PCA9555_0_read(REG_OUTPUT_0);
        temp |= 0x08;   // sbi PORTD, 3 
        PCA9555_0_write(REG_OUTPUT_0,temp);
        _delay_ms(1); 
        temp = PCA9555_0_read(REG_OUTPUT_0);
        temp &= 0b11110111; //cbi PORTD, 3
        PCA9555_0_write(REG_OUTPUT_0,temp);
        _delay_ms(2);
        
        
        lcd_command(0x28);
        lcd_command(0x0c);
        lcd_command(0x01);

        _delay_ms(10);

        lcd_command(0x06);
        
        _delay_ms(10);
        
        return;
} 




int main(void) {
 twi_init();
 uint8_t onoma[11] = "Aristotelis";
 uint8_t eponimo[6] = "Grivas";
 uint8_t onoma2[9] = "Evangelos";
 uint8_t eponimo2[9] = "Mirgiotis";
 uint8_t omada[6] = "Omada:";
 uint8_t noumero[6] = "No. 39";
 unsigned char o,e;
 
 PCA9555_0_write(REG_CONFIGURATION_0, 0x00); //Set EXT_PORT0 as output
 DDRB |= 0x00;
 
 while(1)
 {
     lcd_init();
     
     for(int i = 0; i <11; i++){
       
         lcd_data(onoma[i]);
         
     }
    
     lcd_command(0b11000000);
     
     for(int i = 0; i <6; i++){
         lcd_data(eponimo[i]);
     }
   
 
    _delay_ms(1000); 
 
     lcd_init();
    
    for(int i = 0; i <9; i++){
       
         lcd_data(onoma2[i]);
         
     }
    
     lcd_command(0b11000000);
     
     for(int i = 0; i <9; i++){
         lcd_data(eponimo2[i]);
     }
   
 
    _delay_ms(1000); 
    
    lcd_init();
    
    for(int i = 0; i <6; i++){
       
         lcd_data(omada[i]);
         
     }
    
     lcd_command(0b11000000);
     
     for(int i = 0; i <6; i++){
         lcd_data(noumero[i]);
     }
   
 
    _delay_ms(1000); 
 } 
}




