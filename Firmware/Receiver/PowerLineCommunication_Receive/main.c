#define F_CPU   16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "spi.h"
#include "spi.c"
#include "uart.h"
#include "uart.c"
#include "main.h"
#include "ST7540.h"
#include "ST7540.c"

#define BUFFER_SIZE 500

volatile uint8_t    spi_buffer[BUFFER_SIZE];
volatile uint16_t   spi_buffer_write_pointer=0;
volatile uint16_t   spi_buffer_read_pointer=0;

void port_init(){
    //Configure Output
    DDR_LED |=  (1<<LED1) | (1<<LED2);
    //Enable external interrupts
    PCICR   =   (1<<PCIE1);
    PCMSK1  =   (1<<PCINT13);
}
//SPI Receive interrupt
ISR(SPI_STC_vect){
    //Add received byte to spi buffer
    spi_buffer[spi_buffer_write_pointer]=SPDR;
    spi_buffer_write_pointer++;
    if (spi_buffer_write_pointer>BUFFER_SIZE-1) spi_buffer_write_pointer=0;
}
//On external interrupt CD_PD Set or Unset SS pin for SPI
ISR(PCINT1_vect){
    if ((PIN_CD_PD & (1<<CD_PD))==0) {
        ss_clear();
    }else{
        ss_set();
    }
}

int main(void){
    volatile uint8_t    filter[3];
    volatile uint8_t    filter_pointer=0;
    uint8_t temp1=0,temp2=0, flag=0, data_byte=0;
    
    //Initilize ports
    port_init();
    //Initilize SPI
    spi_init();
    //Inilitize UART
    uart_init();
    //Initilize ST7540
    ST7540_init();
    //Setup ST7540
    ST7540_setup();
    //Enter data access mode
    data_acc();
    _delay_ms(1);
    //Enter register access mode
    reg_acc();
    //Enter receive mode
    rx_mode();
    //Wait for SPI rising edge
    while ((PIN_SPI  &   (1<<SCK))!=0);
    //Clear SS SPI pin
    ss_clear();
    //SPI Receive 3 bytes from ST7540 register
    spi_receive();
    spi_receive();
    spi_receive();
    //Set SS PIN
    ss_set();
    _delay_ms(1);
    data_acc();
    rx_mode();
    
    //Turn off indication LEDS
    PORT_LED    |=  (1<<LED1);
    PORT_LED    |=  (1<<LED2);
    //Enable Global interrupts
    sei();
    while (1) {
        //If there is received data
        if (spi_buffer_write_pointer!=spi_buffer_read_pointer) {
            //Check which byte of the two (of the protocol) received 
            //Correction code
            if (data_byte==0) {
                
                temp1=spi_buffer[spi_buffer_read_pointer];
                //Check if shifted and correct it
                while ((temp1&0xf0)>0x10) temp1=temp1>>1;
                //After correction shift it to MSBs
                temp1=temp1<<4;
                
                data_byte=1;
                
            }else{
                
                temp2=spi_buffer[spi_buffer_read_pointer];
                //Check if shifted and correct it
                while ((temp2&0xf0)>0x10) temp2=temp2>>1;
                //Clear MSBs
                temp2&=0x0f;
                //OR it with the last received byte and compine the final
                temp2|=temp1;
                
                data_byte=0;

                //Filter the value
                filter[filter_pointer]=temp2;
                filter_pointer++;
                //If filter buffer full filter the values
                if (filter_pointer>2) {
                    
                    filter_pointer=0;
                    //The filter checks three bytes in sinquences to be the same
                    if(filter[0]==filter[1]){
                        temp2=filter[0];
                    }else if(filter[1]==filter[2]){
                        temp2=filter[1];
                    }else if(filter[0]==filter[2]){
                        temp2=filter[0];
                    }else{
                        //Error value
                        temp2='E';
                    }
                    //Put to UART the final value
                    uart_putc(temp2);
                    
                    //Blink led
                    if (flag==0) {
                        PORT_LED    |=  (1<<LED1);
                        flag=1;
                    }else{
                        PORT_LED    &=  ~(1<<LED1);
                        flag=0;
                    }
                }
                temp1=0;
                temp2=0;
            }
            spi_buffer_read_pointer++;
            if (spi_buffer_read_pointer>BUFFER_SIZE-1) spi_buffer_read_pointer=0;
        }

    }
    return 0;
}
