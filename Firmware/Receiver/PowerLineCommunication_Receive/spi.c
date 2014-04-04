#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"

void spi_init(){
    
    uint8_t temp=0;
    
    DDR_SS  |=  (1<<SS_PIN);
    
    //Configure Input
    DDR_SPI &=  ~(1<<SS);
    DDR_SPI &=  ~(1<<MOSI);
    DDR_SPI &=  ~(1<<SCK);
    //Configure output
    DDR_SPI |=  (1<<MISO);
    //SPE: SPI Enable
    //MSB data word transmitt first
    SPCR = (1<<SPIE) | (1<<SPE);
    //Clear spi interrupt flag
    temp=SPSR;
    temp=SPDR;    
}

void spi_transmit(uint8_t cData){
	//Start transmission
	SPDR = cData;
	//Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_receive(){
	//Wait for reception complete
	while(!(SPSR & (1<<SPIF)));
	//Return data register
	return SPDR;
}

void clear_spif(){
    uint8_t temp=0;
    //Clear SPIF
    temp=SPSR;
    temp=SPDR;
}