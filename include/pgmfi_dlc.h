#ifndef PGMFI_DLC_H
#define PGMFI_DLC_H

#include <stdint.h>
#include <pgmfi_decoder.h>

namespace DLC {

#define UART_BAUD 9600
#define QUERY_SIZE 4
#define RX_BUFF_SIZE 128
#define VT_MSG_START 0x01
#define VT_MSG_END 0x04

// Handles the physical interface of the Honda Data Link Connector port.
class Pgmfi_Dlc {
    public:
        Pgmfi_Dlc();
        void begin(uint8_t rx_pin, uint8_t tx_pin);
        void loop(void);
        void query(QueryType type);
        bool available(QueryType type);
        bool data(ECU_Info1 &ecu);
        bool data(ECU_Info2 &ecu);
        bool data(Inverter_Master &inv);
        bool data(Inverter_Slave &inv);
    protected:
        void send_message(uint8_t * msg, size_t len);
        void recieve_message(uint8_t * msg, size_t len);
        uint8_t rx_pin;
        uint8_t tx_pin;
        uint8_t rx_buffer[RX_BUFF_SIZE];
        size_t rx_index;
        ECU_Info1 ecu_info1;
        ECU_Info2 ecu_info2;
        Inverter_Master inv_master;
        Inverter_Slave inv_slave;
        bool msg_available;
        QueryType msg_available_type;
        
};

extern Pgmfi_Dlc dlc_interface;

};
#endif