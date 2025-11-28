#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "pjsip_stub.h"
#include <sstream>
#include <iostream>

// Mock class for capturing stdout
class MockOutputStream {
public:
    MOCK_METHOD(void, write, (const std::string& output));
};

// Test fixture for PJSIP stub tests
class PjsipStubTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect cout to capture output
        original_cout_buffer = std::cout.rdbuf();
        std::cout.rdbuf(test_output.rdbuf());
    }

    void TearDown() override {
        // Restore original cout
        std::cout.rdbuf(original_cout_buffer);
    }

    std::stringstream test_output;
    std::streambuf* original_cout_buffer;
};

// Test pjsip_make_call function
TEST_F(PjsipStubTest, MakeCallValidNumber) {
    std::string test_number = "1234567890";
    
    bool result = pjsip_make_call(test_number);
    
    EXPECT_TRUE(result);
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[PJSIP] Making SIP call to 1234567890"));
}

TEST_F(PjsipStubTest, MakeCallEmptyNumber) {
    std::string empty_number = "";
    
    bool result = pjsip_make_call(empty_number);
    
    EXPECT_TRUE(result); // Current implementation always returns true
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[PJSIP] Making SIP call to"));
}

TEST_F(PjsipStubTest, MakeCallInternationalNumber) {
    std::string intl_number = "+1-555-123-4567";
    
    bool result = pjsip_make_call(intl_number);
    
    EXPECT_TRUE(result);
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("+1-555-123-4567"));
}

// Test pjsip_send_udptl function
TEST_F(PjsipStubTest, SendUdptlValidData) {
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    size_t data_len = sizeof(test_data);
    
    bool result = pjsip_send_udptl(test_data, data_len);
    
    EXPECT_TRUE(result);
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[PJSIP] Sending UDPTL packet (size=5)"));
}

TEST_F(PjsipStubTest, SendUdptlEmptyData) {
    uint8_t* null_data = nullptr;
    size_t zero_len = 0;
    
    bool result = pjsip_send_udptl(null_data, zero_len);
    
    EXPECT_TRUE(result);
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[PJSIP] Sending UDPTL packet (size=0)"));
}

TEST_F(PjsipStubTest, SendUdptlLargeData) {
    std::vector<uint8_t> large_data(1024, 0xAA);
    
    bool result = pjsip_send_udptl(large_data.data(), large_data.size());
    
    EXPECT_TRUE(result);
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[PJSIP] Sending UDPTL packet (size=1024)"));
}

// Test pjsip_hangup function
TEST_F(PjsipStubTest, HangupCall) {
    bool result = pjsip_hangup();
    
    EXPECT_TRUE(result);
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[PJSIP] Hanging up SIP call"));
}

// Integration test for complete call flow
TEST_F(PjsipStubTest, CompleteCallFlow) {
    std::string number = "5551234567";
    uint8_t fax_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    
    // Make call
    EXPECT_TRUE(pjsip_make_call(number));
    
    // Send fax data
    EXPECT_TRUE(pjsip_send_udptl(fax_data, sizeof(fax_data)));
    
    // Hangup
    EXPECT_TRUE(pjsip_hangup());
    
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("Making SIP call to 5551234567"));
    EXPECT_THAT(output, ::testing::HasSubstr("Sending UDPTL packet (size=4)"));
    EXPECT_THAT(output, ::testing::HasSubstr("Hanging up SIP call"));
}

// Performance test for multiple UDPTL sends
TEST_F(PjsipStubTest, MultipleUdptlSends) {
    uint8_t test_data[] = {0x01, 0x02, 0x03};
    
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(pjsip_send_udptl(test_data, sizeof(test_data)));
    }
    
    std::string output = test_output.str();
    // Count occurrences of the UDPTL message
    size_t count = 0;
    size_t pos = 0;
    std::string search_str = "Sending UDPTL packet";
    while ((pos = output.find(search_str, pos)) != std::string::npos) {
        count++;
        pos += search_str.length();
    }
    EXPECT_EQ(count, 100);
}
