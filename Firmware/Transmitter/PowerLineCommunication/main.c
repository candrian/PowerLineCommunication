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

void port_init(){
    //Configure Output
    DDR_LED |=  (1<<LED1) | (1<<LED2);
}

int main(void){
    uint8_t flag=0;
    
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
        //If there is no data on the Mains start transmittion
        if ((PIN_CD_PD & (1<<CD_PD))!=0) {
            //Check for available data on UART's buffer
            if (uart_buffer_write_pointer!=uart_buffer_read_pointer) {
                //ST540 transmit
                ST7540_putc(uart_buffer[uart_buffer_read_pointer]);
                //Blink led
                if (flag==0) {
                    PORT_LED    |=  (1<<LED1);
                    flag=1;
                }else{
                    PORT_LED    &=  ~(1<<LED1);
                    flag=0;
                }

                uart_buffer_read_pointer++;
                if (uart_buffer_read_pointer>BUFFER_SIZE-1) uart_buffer_read_pointer=0;
            }
        }
    }
    
    return 0;
}
