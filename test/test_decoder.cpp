#include <unity.h>
#include <pgmfi_decoder.h>

using namespace DLC;

uint8_t ecu_info1_response[] = {
    0xCB, 0x00, 0x00, 0x00, 0x18, 0x09, 0x64, 0x80, 
    0x00, 0x64, 0x2F, 0x01, 0x03, 0x01, 0xF2, 0x00, 
    0x06, 0x7E, 0x7E
};

uint8_t ecu_info2_response[] = {
    0xCB, 0x30, 0x3C, 0x63, 0x01, 0x8F, 0x0D, 0x90,
    0x02, 0x1A, 0x00, 0x10, 0x0A, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x77
};

uint8_t inverter_master_response[] = {
    0xCB, 0x10, 0x00, 0x14, 0x2F, 0x00, 0x00, 0x03, 
    0x03, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 
    0x06, 0x7E, 0x03
};

uint8_t inverter_slave_response[] = {
    0xCB, 0x20, 0x00, 0x1A, 0x30, 0x00, 0x20, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x72
};

void test_decode_type(void) {
    QueryType msg;
    bool success;
    
    success = PGMFI_Decoder::decode_msg_type(ecu_info1_response, QUERY_DATA_LENGTH, msg);
    TEST_ASSERT_TRUE_MESSAGE(success, "Unsupported message!");
    TEST_ASSERT_EQUAL(QueryType::T_ECU_Info1, msg);

    success = PGMFI_Decoder::decode_msg_type(ecu_info2_response, QUERY_DATA_LENGTH, msg);
    TEST_ASSERT_TRUE_MESSAGE(success, "Unsupported message!");
    TEST_ASSERT_EQUAL(QueryType::T_ECU_Info2, msg);

    success = PGMFI_Decoder::decode_msg_type(inverter_master_response, QUERY_DATA_LENGTH, msg);
    TEST_ASSERT_TRUE_MESSAGE(success, "Unsupported message!");
    TEST_ASSERT_EQUAL(QueryType::T_INV_Master, msg);

    success = PGMFI_Decoder::decode_msg_type(inverter_slave_response, QUERY_DATA_LENGTH, msg);
    TEST_ASSERT_TRUE_MESSAGE(success, "Unsupported message!");
    TEST_ASSERT_EQUAL(QueryType::T_INV_Slave, msg);
}

void test_decode_ecu1(void) {
    ECU_Info1 info;
    bool success = PGMFI_Decoder::decode(ecu_info1_response, QUERY_DATA_LENGTH, info);
    TEST_ASSERT_TRUE_MESSAGE(success, "Decode ECU_Info1 did not decode succesfully.");
    TEST_ASSERT_EQUAL(0, info.runtime_d);
    TEST_ASSERT_EQUAL(0, info.runtime_h);
    TEST_ASSERT_EQUAL(24, info.runtime_m);
    TEST_ASSERT_EQUAL(2404, info.rpm);
    TEST_ASSERT_EQUAL(128, info.battery_voltage);
    TEST_ASSERT_EQUAL(50, info.cylinder_head_temp);
    TEST_ASSERT_EQUAL(37, info.ignition_degrees);
    TEST_ASSERT_EQUAL(126, info.output_voltage);
}

void test_decode_ecu2(void) {
    ECU_Info2 info;
    bool success = PGMFI_Decoder::decode(ecu_info2_response, QUERY_DATA_LENGTH, info);
    TEST_ASSERT_TRUE_MESSAGE(success, "Decode ECU_Info1 did not decode succesfully.");
    TEST_ASSERT_EQUAL(99, info.barometric_pressure);
    TEST_ASSERT_EQUAL(399, info.fuel_injection_time);
    TEST_ASSERT_EQUAL(13, info.throttle_position);
    TEST_ASSERT_EQUAL(538, info.output_power);
    TEST_ASSERT_EQUAL(16, info.starter_count);
    TEST_ASSERT_EQUAL(10, info.o2);
}

void test_decode_inv_master(void) {
    Inverter_Master info;
    bool success = PGMFI_Decoder::decode(inverter_master_response, QUERY_DATA_LENGTH, info);
    TEST_ASSERT_TRUE_MESSAGE(success, "Decode Inverter master did not decode succesfully.");
    TEST_ASSERT_EQUAL(20, info.amperage);
    TEST_ASSERT_EQUAL(7, info.temperature);
    TEST_ASSERT_EQUAL(3, info.mode);
    TEST_ASSERT_EQUAL(230, info.watts);
    TEST_ASSERT_EQUAL(126, info.voltage);
}

void test_decode_inv_slave(void) {
    Inverter_Slave info;
    bool success = PGMFI_Decoder::decode(inverter_slave_response, QUERY_DATA_LENGTH, info);
    TEST_ASSERT_TRUE_MESSAGE(success, "Decode Inverter slave did not decode succesfully.");
    TEST_ASSERT_EQUAL(26, info.amperage);
    TEST_ASSERT_EQUAL(8, info.temperature);
    TEST_ASSERT_EQUAL(320, info.watts);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_decode_type);
    RUN_TEST(test_decode_ecu1);
    RUN_TEST(test_decode_ecu2);
    RUN_TEST(test_decode_inv_master);
    RUN_TEST(test_decode_inv_slave);
    UNITY_END();
}

void loop() {

}

#ifdef UNIT_TEST
int main() {
    setup();
}
#endif

