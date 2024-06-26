#include <pgmfi_dlc.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include <stdio.h>

using namespace DLC;


Pgmfi_Dlc::Pgmfi_Dlc(): rx_index(0), msg_available(false) {}

void Pgmfi_Dlc::begin(uint8_t rx_pin, uint8_t tx_pin) {
    this->rx_pin = rx_pin;
    this->tx_pin = tx_pin;

    pinMode(rx_pin, INPUT);
    pinMode(tx_pin, OUTPUT);

    Serial1.begin(UART_BAUD, SERIAL_8N1, rx_pin, tx_pin, true);
}

void Pgmfi_Dlc::loop(void) {
    if (!Serial1.available()) {
        return;
    }
    while (true) {
        int current_byte = Serial1.read();
        if (current_byte == -1) {
            return;
        }
        
        uint8_t byte = (uint8_t)current_byte;

        if (byte == VT_MSG_START) {
            rx_index = 0;
            return;
        } else if(byte == VT_MSG_END) {
            recieve_message(rx_buffer, rx_index);
        } else {
            rx_buffer[rx_index] = byte;
            rx_index += 1;
        }
    }
}

void Pgmfi_Dlc::recieve_message(uint8_t * msg, size_t len) {
    Serial.write("RX: ");
    Serial.write(msg, len);
    Serial.write("\n");
    if (len % 2 != 0)
        // It must be dividible by 2 to be a valid hex message.
        return;
    // This should be the message in Hex excluding the terinal begin/end.
    size_t binary_len = len / 2;
    uint8_t binary_msg[binary_len];
    char * msg_ptr = (char*)msg;
    // convert it from hex to binary
    for (size_t i = 0; i < binary_len; i++) {
        sscanf(msg_ptr, "%2hhX", &binary_msg[i]);
        msg_ptr += 2;
    }

    // great lets decode it.
    QueryType query_type;
    if (!PGMFI_Decoder::decode_msg_type(binary_msg, binary_len, query_type))
        return;
    
    bool success = false;
    
    switch (query_type) {
        case QueryType::T_ECU_Info1:
            success = PGMFI_Decoder::decode(binary_msg, binary_len, ecu_info1);
            break;
        case QueryType::T_ECU_Info2:
            success = PGMFI_Decoder::decode(binary_msg, binary_len, ecu_info2);
            break;
        case QueryType::T_INV_Master:
            success = PGMFI_Decoder::decode(binary_msg, binary_len, inv_master);
            break;
        case QueryType::T_INV_Slave:
            success = PGMFI_Decoder::decode(binary_msg, binary_len, inv_slave);
            break;
        default:
            break;
    }

    if (!success)
        return;
    
    // The results can be obtained by the data methods.
    msg_available_type = query_type;
    msg_available = true;
}

void Pgmfi_Dlc::send_message(uint8_t * msg, size_t len) {
    // Takes a binary string and converts it to Hex and sents it as a terminal message.
    // bin to ascii = *2. + a header, end, and null terminator.
    char tx_buff[(len*2)+3];

    tx_buff[0] = VT_MSG_START;
    size_t index = 1;

    for(size_t i = 0; i < len; i++) {
        // convert it to hex.
        sprintf(tx_buff + index, "%02X", msg[i]);
        index += 2;
    }
    tx_buff[index++] = VT_MSG_END;
    tx_buff[index] = 0x00;

    Serial1.write(tx_buff);
}

void Pgmfi_Dlc::query(QueryType type) {
    uint8_t msg[QUERY_SIZE];
    msg[0] = QUERY_START;
    msg[1] = type;
    msg[2] = 0x00;

    // The last byte i think is a checksum but i dunno. Would be nice to figure that out.
    switch (type) {
        case QueryType::T_ECU_Info1:
            msg[3] = 0x01;
            break;
        case QueryType::T_ECU_Info2:
            msg[3] = 0x02;
            break;
        case QueryType::T_INV_Master:
            msg[3] = 0x00;
            break;
        case QueryType::T_INV_Slave:
            msg[3] = 0x03;
            break;
        default:
            break;
    }

    send_message(msg, QUERY_SIZE);
}

bool Pgmfi_Dlc::available(QueryType type) {
    if (msg_available && msg_available_type == type)
        return true;
    return false;
}

bool Pgmfi_Dlc::data(ECU_Info1 &ecu) {
    if (!(msg_available && msg_available_type == QueryType::T_ECU_Info1))
        return false;
    
    msg_available = false;
    ecu = ecu_info1;
    return true;
}

bool Pgmfi_Dlc::data(ECU_Info2 &ecu) {
    if (!(msg_available && msg_available_type == QueryType::T_ECU_Info2))
        return false;
    
    msg_available = false;
    ecu = ecu_info2;
    return true;
}

bool Pgmfi_Dlc::data(Inverter_Master &inv) {
    if (!(msg_available && msg_available_type == QueryType::T_INV_Master))
        return false;
    
    msg_available = false;
    inv = inv_master;
    return true;
}

bool Pgmfi_Dlc::data(Inverter_Slave &inv) {
    if (!(msg_available && msg_available_type == QueryType::T_INV_Slave))
        return false;
    
    msg_available = false;
    inv = inv_slave;
    return true;
}



Pgmfi_Dlc DLC::dlc_interface;
