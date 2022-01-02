#include <Arduino.h>
#include <HardwareSerial.h>
#include <pgmfi_dlc.h>


#define RX_PIN 26
#define TX_PIN 25

DLC::QueryType current_query;


void print_ecu_info1(void) {
    Serial.printf("--- ECU Info1 ---\n");
    DLC::ECU_Info1 info;
    if (!DLC::dlc_interface.data(info)){
        Serial.write("ECU info data aquisition failed?");
    } else {
        Serial.printf("Bat: %u.%uv\n", info.battery_voltage/10, info.battery_voltage%10);
        Serial.printf("RPM: %u\n", info.rpm);
        Serial.printf("Runtime: %02uD%02uH%02um\n", info.runtime_d, info.runtime_h, info.runtime_m);
        Serial.printf("CylHead: %d c\n", info.cylinder_head_temp);
        Serial.printf("Timing %d deg\n", info.ignition_degrees);
        Serial.printf("OutputV %uv\n", info.output_voltage);
        Serial.printf("\n");
    }
}

void print_ecu_info2(void) {
    Serial.printf("--- ECU Info2 ---\n");
    DLC::ECU_Info2 info;
    if (!DLC::dlc_interface.data(info)){
        Serial.write("ECU info2 data aquisition failed?");
    } else {
        Serial.printf("Baro: %u kpa\n", info.barometric_pressure);
        Serial.printf("Injection Time: %u.%ums\n", info.fuel_injection_time/100, info.fuel_injection_time%100);
        Serial.printf("Throttle Pos: %u%%\n", info.throttle_position);
        Serial.printf("Output Power: %u va\n", info.output_power);
        Serial.printf("Starter Count: %u\n", info.starter_count);
        Serial.printf("o2: %u.%uv\n", info.o2/10, info.o2%10);
        Serial.printf("\n");
    }
}

void print_inv_master(void) {
    Serial.printf("--- Inverter Master ---\n");
    DLC::Inverter_Master inv;
    if (!DLC::dlc_interface.data(inv)) {
        Serial.write("Inv Master data aquisition failed?");
    } else {
        Serial.printf("Current: %ua\n", inv.amperage);
        Serial.printf("Temp: %dc\n", inv.temperature);
        Serial.printf("Mode: %u\n", inv.mode);
        Serial.printf("Power: %uw\n", inv.watts);
        Serial.printf("Volts: %uv\n", inv.voltage);
        Serial.printf("\n");
    }
}

void print_inv_slave(void) {
    Serial.printf("--- Inverter Slave ---\n");
    DLC::Inverter_Slave inv;
    if (!DLC::dlc_interface.data(inv)) {
        Serial.write("Inv Slave data aquisition failed?");
    } else {
        Serial.printf("Current: %ua\n", inv.amperage);
        Serial.printf("Temp: %dc\n", inv.temperature);
        Serial.printf("Power: %uw\n", inv.watts);
        Serial.printf("\n");
    }
}

void setup() {
    Serial.begin(115200);
    
    DLC::dlc_interface.begin(RX_PIN, TX_PIN);

    Serial.write("Start!\n");
    DLC::dlc_interface.query(DLC::QueryType::T_ECU_Info1);
}

void loop() {
    // Check the interface for data.
    DLC::dlc_interface.loop();

    DLC::QueryType test_type;
    DLC::QueryType next_type;
    void (*printfnc)(void);

    for (size_t i = 0; i < 4; i++) {
        switch(i) {
            case 0:
                test_type = DLC::QueryType::T_ECU_Info1;
                next_type = DLC::QueryType::T_ECU_Info2;
                printfnc = print_ecu_info1;
                break;
            case 1:
                test_type = DLC::QueryType::T_ECU_Info2;
                next_type = DLC::QueryType::T_INV_Master;
                printfnc = print_ecu_info2;
                break;
            case 2:
                test_type = DLC::QueryType::T_INV_Master;
                next_type = DLC::QueryType::T_INV_Slave;
                printfnc = print_inv_master;
                break;
            case 3:
                test_type = DLC::QueryType::T_INV_Slave;
                next_type = DLC::QueryType::T_ECU_Info1;
                printfnc = print_inv_slave;
                break;
            default:
                break;
        }

        if (DLC::dlc_interface.available(test_type)) {
            printfnc();
            delay(300);
            DLC::dlc_interface.query(next_type);
        }
    }
}