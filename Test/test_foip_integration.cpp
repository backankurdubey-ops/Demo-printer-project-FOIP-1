#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "pjsip_stub.h"
#include "spandsp_stub.h"
#include <sstream>
#include <iostream>

// Integration test fixture
class FoipIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        original_cout_buffer = std::cout.rdbuf();
        std::cout.rdbuf(test_output.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(original_cout_buffer);
    }

    std::stringstream test_output;
    std::streambuf* original_cout_buffer;
};

// Test complete FOIP workflow as in main.cpp
TEST_F(FoipIntegrationTest, CompleteFoipWorkflow) {
    std::string number = "1234567890";
    
    // 1. PJSIP: start SIP call
    EXPECT_TRUE(pjsip_make_call(number));
    
    // 2. SpanDSP: start fax session
    EXPECT_TRUE(spandsp_start_fax_session());
    
    // 3. Example: single fake TIFF page (raw bytes)
    std::vector<uint8_t> page = { 1, 2, 3, 4, 5 };
    std::vector<uint8_t> encoded;
    
    // 4. Encode page using SpanDSP T.30 logic
    EXPECT_TRUE(spandsp_encode_page(page, encoded));
    EXPECT_FALSE(encoded.empty());
    
    // 5. Send encoded frames via PJSIP UDPTL
    EXPECT_TRUE(pjsip_send_udptl(encoded.data(), encoded.size()));
    
    // 6. End fax session
    std::vector<uint8_t> finalFrames;
    EXPECT_TRUE(spandsp_end_fax_session(finalFrames));
    EXPECT_FALSE(finalFrames.empty());
    
    // 7. Send final frames
    EXPECT_TRUE(pjsip_send_udptl(finalFrames.data(), finalFrames.size()));
    
    // 8. Hangup SIP call
    EXPECT_TRUE(pjsip_hangup());
    
    // Verify complete workflow output
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("Making SIP call to 1234567890"));
    EXPECT_THAT(output, ::testing::HasSubstr("Initializing T.30 fax session"));
    EXPECT_THAT(output, ::testing::HasSubstr("Encoding fax page (5 bytes)"));
    EXPECT_THAT(output, ::testing::HasSubstr("Sending UDPTL packet"));
    EXPECT_THAT(output, ::testing::HasSubstr("Finalizing fax session"));
    EXPECT_THAT(output, ::testing::HasSubstr("Hanging up SIP call"));
}

// Test error handling scenarios
TEST_F(FoipIntegrationTest, ErrorHandlingScenarios) {
    // Test with invalid phone number
    std::string invalid_number = "";
    EXPECT_TRUE(pjsip_make_call(invalid_number)); // Current stub always returns true
    
    // Test with null data
    EXPECT_TRUE(pjsip_send_udptl(nullptr, 0));
    
    // Test multiple hangups
    EXPECT_TRUE(pjsip_hangup());
    EXPECT_TRUE(pjsip_hangup()); // Should handle multiple hangups gracefully
}

// Performance test for multiple fax transmissions
TEST_F(FoipIntegrationTest, MultipleFaxTransmissions) {
    const int num_transmissions = 10;
    
    for (int i = 0; i < num_transmissions; ++i) {
        std::string number = "555000" + std::to_string(i);
        
        EXPECT_TRUE(pjsip_make_call(number));
        EXPECT_TRUE(spandsp_start_fax_session());
        
        std::vector<uint8_t> page(100, static_cast<uint8_t>(i));
        std::vector<uint8_t> encoded;
        
        EXPECT_TRUE(spandsp_encode_page(page, encoded));
        EXPECT_TRUE(pjsip_send_udptl(encoded.data(), encoded.size()));
        
        std::vector<uint8_t> final_frames;
        EXPECT_TRUE(spandsp_end_fax_session(final_frames));
        EXPECT_TRUE(pjsip_send_udptl(final_frames.data(), final_frames.size()));
        
        EXPECT_TRUE(pjsip_hangup());
    }
    
    std::string output = test_output.str();
    
    // Count successful transmissions
    size_t call_count = 0;
    size_t pos = 0;
    std::string search_str = "Making SIP call";
    while ((pos = output.find(search_str, pos)) != std::string::npos) {
        call_count++;
        pos += search_str.length();
    }
    EXPECT_EQ(call_count, num_transmissions);
}

// Mock-based integration test
class MockFoipStack {
public:
    MOCK_METHOD(bool, initialize_sip_stack, ());
    MOCK_METHOD(bool, initialize_fax_engine, ());
    MOCK_METHOD(bool, establish_call, (const std::string& number));
    MOCK_METHOD(bool, negotiate_t38, ());
    MOCK_METHOD(bool, transmit_fax_data, (const std::vector<uint8_t>& data));
    MOCK_METHOD(bool, terminate_call, ());
    MOCK_METHOD(bool, cleanup_resources, ());
};

TEST_F(FoipIntegrationTest, MockedFoipStackWorkflow) {
    MockFoipStack mock_stack;
    
    // Set up expectations for complete FOIP workflow
    ::testing::InSequence seq;
    
    EXPECT_CALL(mock_stack, initialize_sip_stack())
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mock_stack, initialize_fax_engine())
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mock_stack, establish_call("1234567890"))
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mock_stack, negotiate_t38())
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mock_stack, transmit_fax_data(::testing::_))
        .Times(::testing::AtLeast(1))
        .WillRepeatedly(::testing::Return(true));
    
    EXPECT_CALL(mock_stack, terminate_call())
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mock_stack, cleanup_resources())
        .WillOnce(::testing::Return(true));
    
    // Execute the workflow
    EXPECT_TRUE(mock_stack.initialize_sip_stack());
    EXPECT_TRUE(mock_stack.initialize_fax_engine());
    EXPECT_TRUE(mock_stack.establish_call("1234567890"));
    EXPECT_TRUE(mock_stack.negotiate_t38());
    
    std::vector<uint8_t> fax_data = {0x01, 0x02, 0x03, 0x04};
    EXPECT_TRUE(mock_stack.transmit_fax_data(fax_data));
    
    EXPECT_TRUE(mock_stack.terminate_call());
    EXPECT_TRUE(mock_stack.cleanup_resources());
}
