#ifndef PGMFI_DECODER_H
#define PGMFI_DECODER_H


#include <stdint.h>
#include <stddef.h>

#define QUERY_START 0xCB
#define QUERY_ECU1 0x00
#define QUERY_ECU2 0x30
#define QUERY_INV_MASTER 0x10
#define QUERY_INV_SLAVE 0x20
#define QUERY_DATA_LENGTH 19

namespace DLC {


enum QueryType : uint8_t {
    T_ECU_Info1=QUERY_ECU1, 
    T_ECU_Info2=QUERY_ECU2, 
    T_INV_Master=QUERY_INV_MASTER,
    T_INV_Slave=QUERY_INV_SLAVE
};


struct ECU_Info1 {
    // runtime in days
    uint8_t runtime_d;
    // runtime in hours
    uint8_t runtime_h;
    // runtime in minutes
    uint8_t runtime_m;
    uint16_t rpm;
    // voltage in 10th of a volt. ex 121 is 12.1 volts.
    uint8_t battery_voltage;
    // degrees celsius
    int16_t cylinder_head_temp;
    // degrees
    int16_t ignition_degrees;
    // voltage
    uint8_t output_voltage;
};

struct ECU_Info2 {
    // Pressure in KPA
    uint8_t barometric_pressure;
    // * 100 in milliseconds. divide by 100 to get real ms value
    uint16_t fuel_injection_time;
    // Position in %
    uint8_t throttle_position;
    // Power in VA
    uint16_t output_power;
    // Count.
    uint16_t starter_count;
    // sensor value in 0.1 volts
    uint8_t o2;
};

struct Inverter_Master {
    // Amps * 10. Divide by 10 to get real amps.
    uint8_t amperage;
    // Temperature in celsius
    int16_t temperature;
    // Mode. 2 is eco? 3 is not eco?
    uint8_t mode;
    // Output watts.
    uint16_t watts;
    // Output volts.
    uint8_t voltage;
};

struct Inverter_Slave {
    // Amps * 10. Divide by 10 to get real amps.
    uint8_t amperage;
    // Temperature in celsius
    int16_t temperature;
    // Output watts.
    uint16_t watts;
};

class PGMFI_Decoder {
    public:
        // Provided the byte array with a message. Get the type of message.
        static bool decode_msg_type(uint8_t * bytes, size_t len, QueryType &msg_type);
        static bool decode(uint8_t * bytes, size_t len, ECU_Info1 &ecu_info);
        static bool decode(uint8_t * bytes, size_t len, ECU_Info2 &ecu_info);
        static bool decode(uint8_t * bytes, size_t len, Inverter_Master &inv_info);
        static bool decode(uint8_t * bytes, size_t len, Inverter_Slave &inv_info);
};



};
#endif