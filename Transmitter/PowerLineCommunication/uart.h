#define UBRR_2400   416
#define UBRR_9600   103

#define DDR_UART    DDRD
#define PORT_UART   PORTD

#define UART_RX     PD0
#define UART_TX     PD1

#define BUFFER_SIZE 500

volatile uint8_t uart_buffer[BUFFER_SIZE];
volatile uint16_t uart_buffer_write_pointer=0;
volatile uint16_t uart_buffer_read_pointer=0;