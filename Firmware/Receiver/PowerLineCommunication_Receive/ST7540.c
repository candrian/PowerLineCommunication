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