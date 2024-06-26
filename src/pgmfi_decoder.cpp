#include <pgmfi_decoder.h>

using namespace DLC;


bool PGMFI_Decoder::decode_msg_type(uint8_t * bytes, size_t len, QueryType &msg_type) {
    if (len < 2)
        return false;
    
    if (bytes[0] != QUERY_START)
        // Only support decoding messages as a response to a data query.
        return false;
    
    bool success = true;

    switch (bytes[1]) {
        case QUERY_ECU1:
            msg_type = QueryType::T_ECU_Info1;
            break;
        case QUERY_ECU2:
            msg_type = QueryType::T_ECU_Info2;
            break;
        case QUERY_INV_MASTER:
            msg_type = QueryType::T_INV_Master;
            break;
        case QUERY_INV_SLAVE:
            msg_type = QueryType::T_INV_Slave;
            break;
        default:
            success = false;
            break;
    }

    return success;
}

bool PGMFI_Decoder::decode(uint8_t * bytes, size_t len, ECU_Info1 &ecu_info) {
    if (len != QUERY_DATA_LENGTH)
        return false;
    
    if (bytes[1] != QUERY_ECU1)
        return false;
    
    // runtime days
    ecu_info.runtime_d = bytes[2];
    // runtime hours
    ecu_info.runtime_h = bytes[3];
    // runtime minutes
    ecu_info.runtime_m = bytes[4];
    // engine rpm
    ecu_info.rpm = (bytes[5] << 8) | bytes[6];
    // battery voltage in 10th of a volt.
    ecu_info.battery_voltage = bytes[7];
    // cylinder head temp in C
    ecu_info.cylinder_head_temp = ((bytes[8] << 8) | bytes[9]) - 50;
    // iginition timing in degrees
    ecu_info.ignition_degrees = bytes[10] - 10;
    // volts
    ecu_info.output_voltage = bytes[17];

    return true;
}

bool PGMFI_Decoder::decode(uint8_t * bytes, size_t len, ECU_Info2 &ecu_info) {
    if (len != QUERY_DATA_LENGTH)
        return false;
    
    if (bytes[1] != QUERY_ECU2)
        return false;

    // pressure in kpa
    ecu_info.barometric_pressure = bytes[3];
    // fuel injection time in ms * 100
    ecu_info.fuel_injection_time = (bytes[4] << 8) | bytes[5];
    // throttle position. %
    ecu_info.throttle_position = bytes[6];
    // output power in va
    ecu_info.output_power = (bytes[8] << 8) | bytes[9];
    // starting count
    ecu_info.starter_count = (bytes[10] << 8) | bytes[11];
    // o2 sensor * 10 volts.
    ecu_info.o2 = bytes[12];

    return true;
}

bool PGMFI_Decoder::decode(uint8_t * bytes, size_t len, Inverter_Master &inv_info) {
    if (len != QUERY_DATA_LENGTH)
        return false;
    
    if (bytes[1] != QUERY_INV_MASTER)
        return false;
    
    inv_info.amperage = bytes[3];
    inv_info.temperature = bytes[4] - 40;
    inv_info.mode = bytes[8];
    inv_info.watts = ((bytes[10] << 8) | bytes[11]) * 10;
    inv_info.voltage = bytes[17];
    
    return true;
}

bool PGMFI_Decoder::decode(uint8_t * bytes, size_t len, Inverter_Slave &inv_info) {
    if (len != QUERY_DATA_LENGTH)
        return false;
    
    if (bytes[1] != QUERY_INV_SLAVE)
        return false;

    inv_info.amperage = bytes[3];
    inv_info.temperature = bytes[4] - 40;
    inv_info.watts = ((bytes[5] << 8) | bytes[6]) * 10;

    return true;
}

