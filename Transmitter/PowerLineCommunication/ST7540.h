#define DDR_BU_THERM    DDRC
#define DDR_RXTX        DDRC
#define DDR_REG_DATA    DDRC
#define DDR_CD_PD       DDRC

#define PORT_BU_THERM   PORTC
#define PORT_RXTX       PORTC
#define PORT_REG_DATA   PORTC
#define PORT_CD_PD      PORTC

#define PIN_CD_PD       PINC

#define BU_THERM        PC0
#define RXTX            PC1
#define REG_DATA        PC4
#define CD_PD           PC5

#define reg_acc()       PORT_REG_DATA   |=  (1<<REG_DATA);
#define data_acc()      PORT_REG_DATA   &=  ~(1<<REG_DATA);

#define tx_mode()       PORT_RXTX   &=  ~(1<<RXTX);
#define rx_mode()       PORT_RXTX   |=  (1<<RXTX);