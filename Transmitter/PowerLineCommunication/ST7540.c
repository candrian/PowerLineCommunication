#include <avr/io.h>

#include "spi.h"
#include "ST7540.h"

void ST7540_init(){
    //Configure output
    DDR_RXTX        |=  (1<<RXTX);
    DDR_REG_DATA    |=  (1<<REG_DATA);
    //Configure input
    DDR_CD_PD       &=  ~(1<<CD_PD);
    DDR_BU_THERM    &=  ~(1<<BU_THERM);
    ss_set();
}

void ST7540_setup(){
    reg_acc();
    tx_mode();
    
    while ((PIN_SPI  &   (1<<SCK))!=0);
    
    ss_clear();
    //Send configuration bytes to ST7540 control register
    spi_transmit(0x13);
    spi_transmit(0xB2);
    spi_transmit(0x32);
    ss_set();
}

void ST7540_transmit(uint8_t temp){
    //Set the appropriate flags and transmit
    tx_mode();
    ss_clear();
    spi_transmit(temp);
    ss_set();
    rx_mode();
}

void ST7540_putc(uint8_t temp){
    uint8_t temp1=0, temp2=0, i=0;

    //Before transmittion add 0001 header for 
    //shift correction protocol
    
    //First configure MSB
    temp1=temp>>4;
    temp1|=0x10;
    //Then Configure LSB
    temp2=temp&0x0f;
    temp2|=0x10;
    
    //Transmit each original byte 3 times 
    //for failure correction
    for (i=0; i<3; i++) {
        ST7540_transmit(temp1);
        ST7540_transmit(temp2);       
    }

}