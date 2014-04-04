#include <stdio.h>
#include "ST7540.h"
#include "main.h"

volatile uint8_t temp=0;

void uart_init() {
	//Set baudrate counter
    UBRR0H = UBRR_2400>>8;  
    UBRR0L = UBRR_2400;
    //Receiver Transmitter Enable, Complete Interrupt Enabe
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);    
    //Data 8-bit / 1-stop bit / Parity Disabled / Asynchronous mode
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);   
}

//Receive Interrupt
ISR(USART_RX_vect){
    //Read received data and add to buffer
    uart_buffer[uart_buffer_write_pointer]=UDR0;
    uart_buffer_write_pointer++;
    if (uart_buffer_write_pointer>BUFFER_SIZE-1) uart_buffer_write_pointer=0;
}

//Put character
uint8_t uart_putc(char c){
    //Wait until byte sent
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

//Put string
void uart_puts(const char* str){
    while(*str)
        uart_putc(*str++);
}

//Put word decimal
void uart_putw_dec(uint16_t w){
    uint16_t num = 10000;
    uint8_t started = 0;
    
    while(num > 0){
        uint8_t b = w / num;
        if(b > 0 || started || num == 1){
            uart_putc('0' + b);
            started = 1;
        }
        w -= b * num;
        
        num /= 10;
    }
}