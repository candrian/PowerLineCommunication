#define DDR_SPI     DDRB
#define PORT_SPI    PORTB
#define PIN_SPI     PINB

#define DDR_SS      DDRB
#define PORT_SS     PORTB

#define SS_PIN      PB1
#define SS          PB2
#define MOSI        PB3
#define MISO        PB4
#define SCK         PB5

#define ss_clear()  PORT_SS &=  ~(1<<SS_PIN);
#define ss_set()    PORT_SS |=  (1<<SS_PIN);
